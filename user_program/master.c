#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define BUF_SIZE 512
//add
#define PAGE_NUM 128
size_t get_filesize(const char* filename);//get the size of the input file


int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size, offset = 0, tmp;
	char file_name[50], method[20];
	char *kernel_address = NULL, *file_address = NULL;
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed

	//add
	int file_num, total_size = 0;
	file_num = atoi(argv[1]);
	strcpy(method, argv[file_num + 2]);
	for (int i = 1; i <= file_num; i++){
		offset = 0;
		strcpy(file_name, argv[i + 1]);
		//fprintf(stderr, "now %s\n", file_name);


		if( (dev_fd = open("/dev/master_device", O_RDWR)) < 0)
		{
			perror("failed to open /dev/master_device\n");
			return 1;
		}
		gettimeofday(&start ,NULL);
		if( (file_fd = open (file_name, O_RDWR)) < 0 )
		{
			perror("failed to open input file\n");
			return 1;
		}

		if( (file_size = get_filesize(file_name)) < 0)
		{
			perror("failed to get filesize\n");
			return 1;
		}

		if(ioctl(dev_fd, 0x12345677) == -1) //0x12345677 : create socket and accept the connection from the slave
		{
			perror("ioclt server create socket error\n");
			return 1;
		}
		//
		void *src, *dst, *pgd;
		int rev, len;
		switch(method[0])
		{
			case 'f': //fcntl : read()/write()
				do
				{
					ret = read(file_fd, buf, sizeof(buf)); // read from the input file
					write(dev_fd, buf, ret);//write to the the device
				}while(ret > 0);
				break;
			case 'm': //add for mmap
	            while (offset < file_size) {
	                if ((src = mmap(NULL, PAGE_NUM * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, offset)) == (void *) -1){
	                    perror("mapping input file");
	                    return 1;
	                }
	                if ((dst = mmap(NULL, PAGE_NUM * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, offset)) == (void *) -1){
	                    perror("mapping output device");
	                    return 1;
	                }
	                len = offset + PAGE_NUM * PAGE_SIZE > file_size ? (file_size - offset) : PAGE_NUM * PAGE_SIZE;
	                memcpy(dst, src, len);

	                //print page descriptor of master device
	                if (offset == 0)
	                	ioctl(dev_fd, 0x12345676, (unsigned long) dst);
	                offset += len;

	                if (munmap(src, PAGE_NUM * PAGE_SIZE) == -1){
						perror("munmap input file");
						return 1;
					}
					if (munmap(dst, PAGE_NUM * PAGE_SIZE) == -1){
						perror("munmap master device");
						return 1;
					}
	                rev = ioctl(dev_fd, 0x12345678, (unsigned long) len);
	                //fprintf(stderr, "master len = %d, offset = %d\n", (int) len, (int) offset);
	            }
	            ioctl(dev_fd, 0x12345678, (unsigned long) 0);//single file transmission end
	            
				break;
		}
		total_size += file_size;
		close(file_fd);

		if(ioctl(dev_fd, 0x12345679) == -1) // end sending data, close the connection
		{
			perror("ioclt server exits error\n");
			return 1;
		}
	}
	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Transmission time: %lf ms, Total file size: %d bytes\n", trans_time, total_size);
	close(dev_fd);
	return 0;
}

size_t get_filesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
