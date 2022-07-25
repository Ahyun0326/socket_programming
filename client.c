#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>	//파일 관리 함수 헤더
#include <arpa/inet.h>
#include <sys/socket.h>	//소켓 프로그래밍 함수 선언
#include <sys/types.h>

int main(int argc,  char **argv){
	//1. socket() 소켓 생성
	//클라이언트 소켓, 파일디스크립터 변수 선언
	int client_sock;
	
	//sockarr_in 구조체 변수 선언
	struct sockaddr_in server_addr;
	int str_len;
	if(argc != 3){
		printf("Usage: %s <IP> <prot>\n", argv[0]);
		exit(1);
	}

	//보내고 받을 버퍼 정의
	char sendmsg[] = "Test_client";
	char recvmsg[1024];
	
	//클라이언트 소켓 TCP/IP 프로토콜 생성
	client_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(client_sock < 0){
		perror("socket creation error\n");
		exit(1);
	}

	//client_sock에 bind로 주소 넣기 위한 밑작업
	memset(&server_addr, 0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	//2. connect()으로 서버소켓에 연결요청
	if(connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		perror("soket connect error\n");
		exit(1);
	}

	//3. read()/write()
	//sendmsg 보내고, recvmsg에 수신된 string 데이터 담기
	write(client_sock, sendmsg, sizeof(sendmsg));
	str_len = read(client_sock, recvmsg, sizeof(recvmsg)-1);
	if(str_len < 0){
		perror("data receptoin error\n");
		exit(1);
	}

	recvmsg[str_len] = 0;
	printf("Received message: %s\n", recvmsg);

	//4. close() 
	close(client_sock);

	return 0;
}
