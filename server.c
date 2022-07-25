#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //파일 관리 함수 헤더
#include <arpa/inet.h>
#include <sys/socket.h>	//소켓 프로그래밍 함수 선언
#include <sys/types.h>

//argc > 메인 함수에 전달되는 데이터의 개수 의미
//char **argv = char *argv[] > 메인 함수에 전달되는 실제적인 데이터로 char형 포인터 배열로 구성, 첫 번째 문자열은 프로그램의 실행경로 
int main(int argc, char **argv){
	//1. socket() 소켓 생성
	//서버, 클라이언트 소켓 파일디스크립터 변수 선언
	int server_sock, client_sock;

 	//sockaddr_in 구조체 변수 선언
   struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
   int client_addr_size;

 	 //보내고 받을 버퍼 정의
    char sendmsg[] = "Test";
    char recvmsg[1024];
    int str_len;
  
    if(argc != 2){
       printf("Usage: %s <port>\n", argv[0]);
       exit(1);
     }
  
    //서버 소켓 TCP/IP 프로토콜 생성
                          //Ipv4   //TCP       //프로토콜 정보
	 server_sock = socket(PF_INET, SOCK_STREAM, 0);
	//-1을 반환할 경우 소켓 생성 실패
	if(server_sock < 0){
		perror("socket creation error\n");
		exit(1);
	}

	//2. bind() 생성한 소켓에 주소 할당
	//server_sock에 bind로 주소 넣기 위한 밑작업
	memset(&server_addr, 0, sizeof(server_addr));	//메모리 초기화
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	//bind()로 소켓에 주소정보 할당
	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("socket bind error\n");
		exit(1);
	}

	//3. listen()	서버소켓으로 오는 클라이언트 연결요청 대기
	//인자 5 >> 연결요청 대기 큐(Queue)의 크기 정보 전달
	//(큐의 크기만큼 클라이언트의 연결요청을 대기시킬 수 있음)
	if(listen(server_sock, 5) < 0){	//실패 시 -1 반환
		perror("socket listen error\n");
		exit(1);
	}

	//4. accpet()	대기상태의 클라이언토 연결 허용
	//accept 하기 위한 클라이언트 구조체 크기 밑작업 및 함수호출
	client_addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
	if(client_sock < 0){	//실패 시 -1 반환
		perror("socket accept arror\n");
		exit(1);
	}

	//5. read()/write()	데이터 송수신
	//인자1.fd
	//read : 데이터를 받을 대상 소켓의 파일 디스크립터(클라이언트)
	//write : 데이터를 보낼 대상 소켓의 파일 디스크립터(클라이언트)
	//인자2.buf
	//read : 데이터를 받을 버퍼의 주소 값
	//write : 데이터를 보낼 버퍼의 주소 값
	//인자3.nbytes
	//read : 수신할 최대 바이트 수 전달
	//write : 송신할 데이터의 바이트 수 전달
	write(client_sock, sendmsg, sizeof(sendmsg));
	str_len = read(client_sock, recvmsg, sizeof(recvmsg)-1);
	if(str_len < 0){
		perror("data reception error\n");
		exit(1);
	}

	recvmsg[str_len] = 0;
	printf("Received message: %s\n", recvmsg);

	//6. close()	연결 종료
	close(client_sock);
	
	return 0;
}
