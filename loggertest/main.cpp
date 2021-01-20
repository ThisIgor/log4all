#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <boost/format.hpp>

std::string composedata() {
    const char * ar[] = {"а", "б", "в", "г", "д", "е", "ё", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с",
                         "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я"};

    uint64_t timestamp =  std::time(0);
    uint contentsize = rand() % 100;
    std::string  content = "";
    for(int i=0; i < contentsize; i++) content += ar[rand() % (sizeof ar/sizeof (char *))];
    std::string message = (boost::format("{\"timestamp\":  %1%, \"content\": \"%2%\"}") % timestamp % content).str();

    return message;
}

int main(int argc, char* argv[])
{
    struct sockaddr_in addr;
    struct hostent* hostinfo;

    int port = 6001;
    if(argc > 1)
        port = atoi(argv[1]);

    int sock = socket(AF_INET, SOCK_STREAM, 0); // создание TCP-сокета

    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int result(-1);
    while(result != 0) {
        result = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
        if (result < 0)
        {
            perror("Подключение...");
            sleep(10);
        }
    }

    for(;;) {
        std::string data = composedata();
        size_t result = send(sock , data.c_str() , data.length() , 0 );
        sleep(1);
    }
}