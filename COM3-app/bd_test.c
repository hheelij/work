

#include "common.h"


typedef struct _Comm_Package_Buffer
{
	struct list_head list;
	HE_UCHAR  *package_buffer;
	HE_U16	package_buffer_len; 
}Comm_Package_Buffer;

typedef int (*Comm_Init_Function)(void);

typedef struct _BD_Test_Comm_Context 
{
	HE_S32 fd_com;
	HE_BOOL init_done ;
	Comm_Init_Function comm_init;
	sem_t  buffer_lock;
	struct list_head buffer_head ;
}BD_Test_Comm_Context;

static BD_Test_Comm_Context debug_comm_context, BD1_comm_context;
static fd_set readFds;
static HE_S32 nfds = 0;
static sem_t  log_to_file_lock;

static HE_S32 write_log(HE_UCHAR *msg);


static HE_S32 debug_comm_init(HE_VOID)
{
	struct termios  ios_dbg;

	if(debug_comm_context.init_done == HE_TRUE)
		return HE_SUCCESS;

	debug_comm_context.fd_com = open("/dev/ttySAC0",O_RDWR);
	if(debug_comm_context.fd_com < 0)
		return HE_FAILURE;

	//串口属性设置(debgu)
    if ( isatty( debug_comm_context.fd_com ) ) 
	{        
      
	  #if 0
		bzero(&ios_dbg, sizeof(ios_dbg));	
	    ios_dbg.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	    ios_dbg.c_iflag = IGNPAR;		// | IGNCR ;//| INLCR;
	    ios_dbg.c_oflag = 0;
	    ios_dbg.c_lflag = ICANON;		//规范输入
	    tcflush(debug_comm_context.fd_com, TCIOFLUSH);
	    fcntl(debug_comm_context.fd_com, F_SETFL,0);		//设置文件状态标志
	    tcsetattr(debug_comm_context.fd_com, TCSANOW, &ios_dbg);	//设置立即生效
	  #endif

		tcgetattr( debug_comm_context.fd_com, &ios_dbg );
        //ios_dbg.c_lflag = 0;  // disable ECHO, ICANON, etc... 
		ios_dbg.c_iflag = IGNBRK;
        ios_dbg.c_cflag = (CLOCAL | CREAD | CS8 | HUPCL);
       
        // set raw input
        ios_dbg.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        ios_dbg.c_iflag &= ~(INLCR | ICRNL | IGNCR);
        
        // set raw output
        ios_dbg.c_oflag &= ~OPOST;
        ios_dbg.c_oflag &= ~OLCUC;
        ios_dbg.c_oflag &= ~ONLRET;
        ios_dbg.c_oflag &= ~ONOCR;
        ios_dbg.c_oflag &= ~OCRNL;

		cfsetispeed(&ios_dbg, B9600);
		cfsetospeed(&ios_dbg, B9600);

        tcsetattr( debug_comm_context.fd_com, TCSANOW, &ios_dbg );
		//tcflush(debug_comm_context.fd_com,TCIOFLUSH);
		write_log("debug_comm_init:set debug comm0 B9600");
    }
	
	debug_comm_context.init_done = HE_TRUE;
	
	return HE_SUCCESS;
}

static HE_S32 BD1_comm_init(HE_VOID)
{
	struct termios  ios_dbg;

	if(BD1_comm_context.init_done == HE_TRUE)
		return HE_SUCCESS;

	BD1_comm_context.fd_com = open("/dev/ttySAC3",O_RDWR);
	if(BD1_comm_context.fd_com < 0)
		return HE_FAILURE;

	//串口属性设置(debgu)
    if ( isatty( BD1_comm_context.fd_com ) ) 
	{        
        tcgetattr( BD1_comm_context.fd_com, &ios_dbg );
		
        ios_dbg.c_iflag = IGNBRK;
        ios_dbg.c_cflag = (CLOCAL | CREAD | CS8 | HUPCL);
       
        // set raw input
        ios_dbg.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        ios_dbg.c_iflag &= ~(INLCR | ICRNL | IGNCR);
        
        // set raw output
        ios_dbg.c_oflag &= ~OPOST;
        ios_dbg.c_oflag &= ~OLCUC;
        ios_dbg.c_oflag &= ~ONLRET;
        ios_dbg.c_oflag &= ~ONOCR;
        ios_dbg.c_oflag &= ~OCRNL;
		
		cfsetispeed(&ios_dbg, B9600);
		cfsetospeed(&ios_dbg, B9600);

        tcsetattr( BD1_comm_context.fd_com, TCSANOW, &ios_dbg );
		tcflush(BD1_comm_context.fd_com,TCIOFLUSH);
    }
	
	BD1_comm_context.init_done = HE_TRUE;

	write_log("BD1_comm_init:set comm2 B9600");
	return HE_SUCCESS;
}

static HE_S32 bd_test_context_init(HE_VOID)
{
	HE_S32 ret;

	debug_comm_context.init_done = HE_FALSE;
	BD1_comm_context.init_done = HE_FALSE;

	debug_comm_context.comm_init = debug_comm_init;
	BD1_comm_context.comm_init = BD1_comm_init;	

	sem_init(&debug_comm_context.buffer_lock,0,1);
	sem_init(&BD1_comm_context.buffer_lock,0,1);	

	INIT_LIST_HEAD(&debug_comm_context.buffer_head);
	INIT_LIST_HEAD(&BD1_comm_context.buffer_head);

	return HE_SUCCESS;
}

static HE_S32 comm_read(HE_S32 fd,HE_CHAR *buf)
{
	HE_CHAR buff[255];
    HE_S32 ret,count;

	ret = 0;
	count = 0;
    do {
        ret = read(fd, &buff[count], sizeof(buff)-count);
		if(ret >0 ) count = count + ret;
    } while (ret < 0 && errno == EINTR);

	if(ret > 0)
		memcpy(buf,buff,ret);

	else
		write_log("comm_read:count error!!");

	return ret;
}

static void getBufferLock(sem_t *lock)
{
	sem_wait(lock);
}

static void releaseBufferLock(sem_t *lock)
{
	sem_post(lock);
}

static inline void addBuffer(Comm_Package_Buffer *add,struct list_head *head )
{
	list_add_tail(&add->list,head);
}

static inline HE_BOOL is_buffer_empty(struct list_head *head)
{
	return (list_empty(head)?HE_TRUE:HE_FALSE);
}

static inline Comm_Package_Buffer *getFirstBuffer(struct list_head *head)
{
	Comm_Package_Buffer *first = list_first_entry(head,Comm_Package_Buffer,list);

	assert(first);

	write_log("get the head package from send buffer ");
	return first;
}

static inline void removeBuffer(Comm_Package_Buffer *del)
{
	list_del(&del->list);

	if(del->package_buffer)
		free(del->package_buffer);

	free(del);
	del = HE_NULL_PTR;	
}


static void add_to_buf_list(HE_UCHAR *buffer, HE_S32 bufferLen,BD_Test_Comm_Context *context)
{
	Comm_Package_Buffer *recvNew;

	if((buffer == HE_NULL_PTR)||(bufferLen == 0))
		{
			write_log("add_to_buf_list:buffer is NULL or len=0");
			return;
		}

	recvNew = (Comm_Package_Buffer *) calloc(1, sizeof(Comm_Package_Buffer));
	if(recvNew == HE_NULL_PTR)
		{
			write_log("add_to_buf_list:Receive package callback error");
			return;
		}

	getBufferLock(&context->buffer_lock);
	
	recvNew->package_buffer = (HE_UCHAR *)malloc(bufferLen);
	assert(recvNew->package_buffer);
	memcpy(recvNew->package_buffer,buffer,bufferLen);
	recvNew->package_buffer_len = bufferLen;	
	addBuffer(recvNew,&context->buffer_head);

	releaseBufferLock(&context->buffer_lock);

}



static HE_S32 write_log(HE_UCHAR *msg)
{
    HE_S32 fd = -1;
    HE_S32 ret = -1;
    time_t t_now;
    struct tm *tm_now = NULL;
    HE_UCHAR time_buf[256];
    HE_UCHAR filename[64] = "/data/bd1_com_test.log";

	sem_wait(&log_to_file_lock);
    time(&t_now);
    tm_now = localtime((const time_t *)&t_now);
    strftime(time_buf,sizeof(time_buf),"%Y%m%d %H%M%S ",tm_now);
    strcat(time_buf,msg);
    ret = strlen(time_buf);
    time_buf[ret] = '\n';
    time_buf[ret + 1] = '\0';

    fd = open(filename, O_CREAT | O_APPEND | O_WRONLY /*| O_SYNC*/, 0777);
    if (fd < 0)
		{
			sem_post(&log_to_file_lock);
			return -1;
		}

    ret = write(fd, time_buf, strlen(time_buf));
    fsync(fd);
    if (ret < 0)
    {
		close(fd);
		sem_post(&log_to_file_lock);
		return -1;
    }

    close(fd);
	sem_post(&log_to_file_lock);
    return 0;
}

static HE_S32 epoll_register( HE_S32  epoll_fd, HE_S32  fd )
{
    struct epoll_event  ev;
    HE_S32 ret, flags;

    /* important: make the fd non-blocking */
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    ev.events  = EPOLLIN;
    ev.data.fd = fd;
    do {
        ret = epoll_ctl( epoll_fd, EPOLL_CTL_ADD, fd, &ev );
    } while (ret < 0 && errno == EINTR);
    return ret;
}

static void *receiverLoop(void *param)
{
	HE_S32 recLen = 0;
	HE_S32 epoll_fd   = epoll_create(2);
	HE_S32 fd;
	HE_UCHAR new_buf[256];

	struct epoll_event   events[2];
    HE_S32   ne, nevents;   

	tcflush(BD1_comm_context.fd_com,TCIOFLUSH);
	tcflush(debug_comm_context.fd_com,TCIOFLUSH);
	epoll_register( epoll_fd, BD1_comm_context.fd_com );
	epoll_register( epoll_fd, debug_comm_context.fd_com );
	

	write_log("comm reader start:");
	while(1)
		{			
			nevents = epoll_wait( epoll_fd, events, 2, -1 );
	        if (nevents < 0) 
			{
	            if (errno == EINTR) continue;
            	write_log("receiverLoop: select error ()");           
           		return;
        	}
			
			 for (ne = 0; ne < nevents; ne++) 
			  {
	            if ((events[ne].events & (EPOLLERR|EPOLLHUP)) != 0) 
				{
	                write_log("EPOLLERR or EPOLLHUP after epoll_wait() !?");
	                return;
	            }

            	if ((events[ne].events & EPOLLIN) != 0) 
				{
					 fd = events[ne].data.fd;
					 if (fd == BD1_comm_context.fd_com) 
						{
							recLen = comm_read(BD1_comm_context.fd_com,new_buf);
							write_log("BD1 COMM1 read msg:");
							write_log(new_buf);
					        add_to_buf_list(new_buf, recLen, &BD1_comm_context);					        
			    		}

					if (fd == debug_comm_context.fd_com) 
						{
							recLen = comm_read(debug_comm_context.fd_com,new_buf);
							write_log("DEBUG COMM0 read msg:");
							write_log(new_buf);
					        add_to_buf_list(new_buf, recLen, &debug_comm_context);			        
			    		}	
				  }					
				}

			}

	return HE_NULL;
}

#if 0
static void *receiverLoop(void *param)
{
	HE_S32 recLen = 0;
	HE_S32 n;
	HE_UCHAR *new_buf;
	struct timeval tvptr;

	tvptr.tv_sec = 3;
    tvptr.tv_usec = 0;
	FD_ZERO(&readFds);
	FD_SET(BD1_comm_context.fd_com, &readFds);
    if (BD1_comm_context.fd_com >= nfds) nfds = BD1_comm_context.fd_com+1;

	FD_SET(debug_comm_context.fd_com, &readFds);
    if (debug_comm_context.fd_com >= nfds)nfds = debug_comm_context.fd_com+1;	

	write_log("comm reader start:");
	while(1)
		{			
			n = select(nfds, &readFds, NULL, NULL, &tvptr);
	        if (n < 0) 
			{
	            if (errno == EINTR) continue;
            	write_log("receiverLoop: select error ()");           
           		return;
        	}

			if (FD_ISSET(BD1_comm_context.fd_com, &readFds)) 
				{
					new_buf = NULL;
					recLen = comm_read(BD1_comm_context.fd_com,new_buf);
					write_log("BD1 COMM1 read msg:");
					write_log(new_buf);
			        add_to_buf_list(new_buf, recLen, &BD1_comm_context);
			        
	    		}

			if (FD_ISSET(debug_comm_context.fd_com, &readFds)) 
				{
					new_buf = NULL;
					recLen = comm_read(debug_comm_context.fd_com,new_buf);
					write_log("DEBUG COMM0 read msg:");
					write_log(new_buf);
			        add_to_buf_list(new_buf, recLen, &debug_comm_context);			        
	    		}			
		}

	return HE_NULL;
}

#endif


static HE_S32 commReceiverStart(HE_VOID)
{	
    HE_S32 ret = HE_SUCCESS;
    pthread_attr_t attr;
	pthread_t s_tid_receiver;

    if( BD1_comm_context.init_done && debug_comm_context.init_done)
		{
		    pthread_attr_init (&attr);
		    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		    ret = pthread_create(&s_tid_receiver, &attr, receiverLoop, NULL);

		    if (ret < 0) {
		        return HE_FAILURE;
		    }	
			return ret;
		}

	else 
		return HE_FAILURE;
}

static HE_S32 blockingWrite(HE_S32 fd, const void *buffer, HE_U32 len) 
{
    HE_U32 writeOffset = 0;
    const HE_CHAR *toWrite;

    toWrite = (const HE_CHAR *)buffer;

    while (writeOffset < len) {
        HE_U32 written;
        do {
            written = write (fd, toWrite + writeOffset,
                                len - writeOffset);
        } while (written < 0 && errno == EINTR);

        if (written >= 0) {
            writeOffset += written;
        } else {   // written < 0
            write_log("blockingWrite: unexpected error on write errno:");
            close(fd);
            return -1;
        }
    }

    return 0;
}


static void *senderLoop(void *param)
{
	Comm_Package_Buffer *buffer;
	HE_S32 ret;
	HE_UCHAR *new_buf;

	write_log("comm writer start:");

	//blockingWrite(debug_comm_context.fd_com,"test for debug com",sizeof("test for debug com"));
	while(1)
		{		
			getBufferLock(&BD1_comm_context.buffer_lock);	
			if(is_buffer_empty(&BD1_comm_context.buffer_head)) //send buffer is empty
			   {
				releaseBufferLock(&BD1_comm_context.buffer_lock);
			   }
			else
				{
					buffer = getFirstBuffer(&BD1_comm_context.buffer_head);
					ret = blockingWrite(debug_comm_context.fd_com,buffer->package_buffer,buffer->package_buffer_len);
					if(ret)
						write_log("blockingWrite: BD1_comm error");
					write_log("Debug COMM0 write msg:");
					write_log(buffer->package_buffer);

					removeBuffer(buffer);			
					releaseBufferLock(&BD1_comm_context.buffer_lock);
				}

			getBufferLock(&debug_comm_context.buffer_lock);	
			if(is_buffer_empty(&debug_comm_context.buffer_head)) //send buffer is empty
			   {
				releaseBufferLock(&debug_comm_context.buffer_lock);
				usleep(100);
				continue;
			   }
			buffer = getFirstBuffer(&debug_comm_context.buffer_head);
			ret = blockingWrite(BD1_comm_context.fd_com,buffer->package_buffer,buffer->package_buffer_len);
			if(ret)
				write_log("blockingWrite: debug_comm_context error");
			write_log("BD1 COMM1 write msg:");
			write_log(buffer->package_buffer);
			removeBuffer(buffer);
			releaseBufferLock(&debug_comm_context.buffer_lock);	

		}

	return HE_NULL;
}


static HE_S32 commSenderStart(HE_VOID)
{	
    HE_S32 ret = HE_SUCCESS;
    pthread_attr_t attr;
	pthread_t s_tid_sender;

    if( BD1_comm_context.init_done && debug_comm_context.init_done)
		{
		    pthread_attr_init (&attr);
		    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		    ret = pthread_create(&s_tid_sender, &attr, senderLoop, NULL);

		    if (ret < 0) {
		        return HE_FAILURE;
		    }	
			return ret;
		}

	else 
		return HE_FAILURE;
}

static HE_VOID loopSleep(HE_VOID)
{
	while(1)
		sleep(0x00ffffff);

	return ;
}

HE_S32 main(HE_VOID)
{
	HE_S32 ret;

	sem_init(&log_to_file_lock,0,1);

	write_log("bd1 com test star~");
	bd_test_context_init();

	ret = debug_comm_context.comm_init();
	if(ret == HE_FAILURE )
		return HE_FAILURE;
	
	ret = BD1_comm_context.comm_init();
	if(ret == HE_FAILURE )
		return HE_FAILURE;	

	ret = commReceiverStart();
	if(ret == HE_FAILURE )
		return HE_FAILURE;	

	ret = commSenderStart();
	if(ret == HE_FAILURE )
		return HE_FAILURE;	

	loopSleep();
	return 1;
}
