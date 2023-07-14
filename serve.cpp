#include<bits/stdc++.h>
#include<winsock2.h>
#include<gmp.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
struct get_key
{
    char * n;
    char * d;
    int e;
};
mpz_t * RSA_por_max_futioin()
{
	mpz_t key_q,key_p;
	mpz_init(key_p);
	mpz_init(key_q);
	gmp_randstate_t sum;
	gmp_randinit_default(sum);
	gmp_randseed_ui(sum,time(NULL));
	mpz_urandomb(key_p,sum,2048);
	mpz_urandomb(key_q,sum,2048);
	mpz_t *ans=new mpz_t[2];
	mpz_init(ans[0]);
	mpz_init(ans[1]);
	mpz_nextprime(ans[0],key_p);
	mpz_nextprime(ans[1],key_q);
	mpz_clear(key_q);
	mpz_clear(key_p);
	return ans;
}//RSA生成 p and q
get_key*get_key_n()
{
    mpz_t *primes =RSA_por_max_futioin();
    mpz_t key_n,key_e,key_n1;
    mpz_t key_d;
    mpz_init(key_n);
    mpz_init(key_n1);
    mpz_init(key_d);
    mpz_init_set_ui(key_e, 65537);
    mpz_mul(key_n,primes [0],primes [1]);
    mpz_sub_ui(primes [0],primes [0],1);
    mpz_sub_ui(primes [1],primes [1],1);
    mpz_mul(key_n1,primes [0],primes [1]);

    mpz_invert(key_d,key_e,key_n1);
    get_key*ans=new get_key;
    char *key_rsa_n=new char [4096];
    char *key_rsa_d=new char [4096];
    mpz_get_str(key_rsa_n,10,key_n);
    ans->n=key_rsa_n;
    mpz_get_str(key_rsa_d,10,key_d);
    ans->d=key_rsa_d;
    ans->e=65537;
    mpz_clear(key_n);
    mpz_clear(key_d);
    mpz_clear(key_n1);
    mpz_clear(key_e);
    mpz_clear(primes[0]);
    mpz_clear(primes[1]);
    delete []primes;

    return ans;
}//RSA生成 e d n
char *RSA_de(const char *text_c,const char *key_n,const char*key_d)
{
    mpz_t m,c,n,d;
    mpz_init_set_str(c,text_c,10);
    mpz_init_set_str(n,key_n,10);
    mpz_init_set_str(d,key_d,10);
    mpz_init(m);
    mpz_powm(m, c, d, n); 
    char *ans=new char [4096];
    mpz_get_str(ans,10,m);
    return ans;
}//RSA解密函数
int transformation(char a[])
{
	char c;
	for(int i=0;i<=strlen(a);i++)
	{
		if(97<=a[i]<=122)
		{
			c=a[i];
			a[i]=c;
		}
	}
}
int main()
{

	get_key *key=get_key_n();

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	
	SOCKET hServSock = socket(PF_INET,SOCK_STREAM,0);//Ipv4 TCP
	
	//绑定

	SOCKADDR_IN servAddr;//IP地址
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(8888);//端口�?114514
	bind(hServSock,(SOCKADDR*)&servAddr,sizeof(servAddr));//hServSock本机IP
	
	//连接,�?

	listen(hServSock,5);
	SOCKADDR_IN acceptAddr;//对方IP
	int acceptAddr_size;
	acceptAddr_size = sizeof(acceptAddr);
	SOCKET hacceptSock = accept(hServSock,(SOCKADDR*)&acceptAddr,&acceptAddr_size);

	//说话
	printf("对方IP为: %s\n",inet_ntoa(acceptAddr.sin_addr));

	//cout<<"私钥为"<<endl;
	//cout<<key->d<<endl;
	char RSA_n[2058];
	strcpy(RSA_n,key->n);
	send(hacceptSock,RSA_n,2058,0);
	//cout<<"65537"<<endl;
	key->n=RSA_n;
	cout<<"公钥为"<<endl;
	cout<<key->n<<endl;
	//send(hacceptSock,"65537",4096,0);

	while(1)
	{
		char message[2058];
		char type[16]={0};
		recv(hacceptSock,type,16,0);
		cout<<type<<endl;
		char type_temporary;
		type_temporary=type[0];
		//type_temporary='2';
		cout<<type_temporary;
		//Sleep(2500000);
		if(type_temporary=='1')
		{
			char transformation_C[2058];
			recv(hacceptSock,message,2048,0);
			cout<<"收到消息"<<endl;
			//cout<<message<<endl;
			char *C_mi=RSA_de(message,key->n,key->d);
			//strcpy(transformation_C,C_mi);
			cout<<C_mi<<endl;

		}
		if(type_temporary=='2')
		{
			char filename[2058];
			char filename_data[2058];
			char* filename_size;
			recv(hacceptSock,filename,128,0);
			FILE *fp = fopen(filename,"wb");
			//recv(hacceptSock,filename_size,114514,0);
			//int filename_size_int=atoi(filename_size);
			//cout<<message<<endl;
			int byt;
			while((byt = recv(hacceptSock,filename_data,2048,0))>0)
			{
				fwrite(filename_data,1,byt,fp);
			}
			cout<<"收到文件"<<endl;
			fclose(fp);
		}
		if(type_temporary=='3')
		{
			break;
		}
	}
	closesocket(hacceptSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}
