# Fonctions réseau et sockets
## socket

    - Utilisation : Crée un point de communication socket.
    - Exemple : int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    - Bibliothèque : #include <sys/socket.h>

## close

    - Utilisation : Ferme un descripteur de fichier ou de socket.
    - Exemple : close(sockfd);
    - Bibliothèque : #include <unistd.h>

## setsockopt

    - Utilisation : Modifie les options d'un socket.
    - Exemple :

```c
    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

```

    - Bibliothèque : #include <sys/socket.h>

## getsockname

    - Utilisation : Obtient l'adresse locale d'un socket.
    - Exemple :

```c
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getsockname(sockfd, (struct sockaddr *)&addr, &len);
```


    - Bibliothèque : #include <sys/socket.h>

## getprotobyname

    - Utilisation : Obtient les informations du protocole par son nom.
    - Exemple : struct protoent *proto = getprotobyname("tcp");
    - Bibliothèque : #include <netdb.h>

## gethostbyname

    - Utilisation : Obtient les informations d'hôte par son nom.
    - Exemple : struct hostent *host = gethostbyname("www.example.com");
    - Bibliothèque : #include <netdb.h>

## getaddrinfo / freeaddrinfo

    - Utilisation : Résout les informations d'adresse à partir d'un nom d'hôte et d'un service.
    - Exemple : Voir la documentation pour un exemple détaillé.
    - Bibliothèque : #include <netdb.h>

## bind

    - Utilisation : Lie un socket à une adresse locale.
    - Exemple :

```c
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

```

    - Bibliothèque : #include <sys/socket.h>

## connect

    - Utilisation : Établit une connexion vers un socket distant.
    - Exemple :

```c
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.1");
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

    - Bibliothèque : #include <sys/socket.h>

## listen

    - Utilisation : Met un socket en mode écoute pour les connexions entrantes.
    - Exemple : listen(sockfd, 5);
    - Bibliothèque : #include <sys/socket.h>

## accept

    - Utilisation : Accepte une connexion entrante et crée un nouveau socket.
    - Exemple :
```c
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);

```

    - Bibliothèque : #include <sys/socket.h>

## htons, htonl, ntohs, ntohl

    - Utilisation : Convertissent les valeurs entre l'ordre d'octets de l'hôte et celui du réseau.
    - Exemple : uint16_t networkShort = htons(hostShort);
    - Bibliothèque : #include <arpa/inet.h>

## inet_addr, inet_ntoa

    - Utilisation : Convertissent les adresses IP entre formats texte et binaire.
    - Exemple :
```c
    in_addr_t addr = inet_addr("192.168.1.1");
    char *ipStr = inet_ntoa(addr);

```

    - Bibliothèque : #include <arpa/inet.h>

## send, recv

    - Utilisation : Envoient et reçoivent des données sur une socket.
    - Exemple :
```c
    char buffer[1024];
    ssize_t bytes_sent = send(sockfd, buffer, sizeof(buffer), 0);
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);

```

    - Bibliothèque : #include <sys/socket.h>

# Autres fonctions
## signal, sigaction

    - Utilisation : Gèrent les signaux logiciels envoyés au processus.
    - Exemple : Voir la documentation pour des exemples spécifiques.
    - Bibliothèque : #include <signal.h>

## lseek

    - Utilisation : Déplace le curseur de lecture/écriture dans un fichier.
    - Exemple : off_t new_offset = lseek(file_descriptor, 0, SEEK_SET);
    - Bibliothèque : #include <unistd.h>

## fstat

    - Utilisation : Obtient les informations d'état d'un fichier.
    - Exemple :
```c
    struct stat file_stat;
    fstat(file_descriptor, &file_stat);
```

    - Bibliothèque : #include <sys/stat.h>

## fcntl

    - Utilisation : Effectue diverses opérations sur un descripteur de fichier.
    - Exemple : Voir la documentation pour des exemples spécifiques.
    - Bibliothèque : #include <fcntl.h>

## poll (ou équivalent)

    - Utilisation : Gère les opérations d'entrée/sortie multiplexées.
    - Exemple : Voir la documentation pour des exemples spécifiques.
    - Bibliothèque : #include <poll.h>