#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "funcoes.h"

//-------Defines----------
#define MAX_USERNAME 40
#define MAX_PASSWORD 40
#define SEMANAS 5
#define DIAS_DA_SEMANA 7
#define NUM_QUARTOS 4
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define YELLOW "\033[0;33m"

#ifdef _WIN32
#include <conio.h> // getch() é usado no Windows
#else
#include <termios.h>
#endif

//-------------------ESTRUTURA DO PROJETO--------------------|

// Quick Sort - (Algoritmo de Ordenação -> Ln 94 - Ln 128
// Funcões de Validações -> Ln 131 - Ln 228
// Funções dos menus -> Ln 232 - Ln 271
// Funções de Login -> Ln 411 - Ln 597

/*Funçõs do Cliente
- Funções para fazer reserva -> Ln 608 - Ln 849
- Funções para avaliar-nos -> Ln 888 - Ln 896
- Funções para consultar reserva -> Ln 896 - 1034
*/

//Funções de ADM -> Ln 1045 Ln 1198

//------------------------------------------------------|

//-------Estruturas-------

typedef struct
{
    char nome[MAX_USERNAME];
    char palavraPasse[MAX_PASSWORD];
    int numContribuinte;
} Utilizador;

typedef struct
{
    int checkIn;
    int checkOut;
    int numContribuinte;
    char tipoQuarto[20];
    int quarto[NUM_QUARTOS];
} Reserva;

typedef struct
{
    int avaliacao;
    char nomeUser[MAX_USERNAME];
} Feedback;

// Variáveis globais (podem ser acessadas e manipuladas em qualquer função do programa)

char *meses[] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};
int mesAtual = 0; // começa em Janeiro (primeiro mês)
char userLogado[MAX_USERNAME];

Feedback feedback[150];
int countAvaliacao = 0;

//-------Implementação das Funções-------

void handle_sigint(int sig)
{
    printf(RED "\nNão é permitido ctrl + c" RESET);
    fflush(stdout); // Volta ao menu inicial
    return;
}

void clear()
{
    system("cls");
}

void art()
{
    printf("   $$$$$\\ $$\\             $$\\       $$$$$$\\  $$$$$$\\  $$$$$$$\\   $$$$$$\\  $$\\   $$\\       $$\\   $$\\  $$$$$$\\ $$$$$$$$\\ $$$$$$$$\\ $$\\       \n");
    printf("   \\__$$ |$$ |            $$ |      \\_$$  _|$$  __$$\\ $$  __$$\\ $$  __$$\\ $$$\\  $$ |      $$ |  $$ |$$  __$$\\\\__$$  __|$$  _____|$$ |      \n");
    printf("      $$ |$$ |            $$ |        $$ |  $$ /  \\__|$$ |  $$ |$$ /  $$ |$$$$\\ $$ |      $$ |  $$ |$$ /  $$ |  $$ |   $$ |      $$ |      \n");
    printf("      $$ |$$ |            $$ |        $$ |  \\$$$$$$\\  $$$$$$$\\ |$$ |  $$ |$$ $$\\$$ |      $$$$$$$$ |$$ |  $$ |  $$ |   $$$$$\\    $$ |      \n");
    printf("$$\\   $$ |$$ |            $$ |        $$ |   \\____$$\\ $$  __$$\\ $$ |  $$ |$$ \\$$$$ |      $$  __$$ |$$ |  $$ |  $$ |   $$  __|   $$ |      \n");
    printf("$$ |  $$ |$$ |            $$ |        $$ |  $$\\   $$ |$$ |  $$ |$$ |  $$ |$$ |\\$$$ |      $$ |  $$ |$$ |  $$ |  $$ |   $$ |      $$ |      \n");
    printf("\\$$$$$$  |$$$$$$$$\\       $$$$$$$$\\ $$$$$$\\ \\$$$$$$  |$$$$$$$  | $$$$$$  |$$ | \\$$ |      $$ |  $$ | $$$$$$  |  $$ |   $$$$$$$$\\ $$$$$$$$\\ \n");
    printf(" \\______/ \\________|      \\________|\\______| \\______/ \\_______/  \\______/ \\__|  \\__|      \\__|  \\__| \\______/   \\__|   \\________|\\________|\n");
}

//-------------------------------Quick Sort - (Algoritmo de Ordenação)-------------------------------

void trocar(Utilizador *usersA, Utilizador *usersB)
{
    Utilizador temp = *usersA;
    *usersA = *usersB;
    *usersB = temp;
}

int particionar(Utilizador *users, int inicio, int fim)
{
    Utilizador pivo = users[fim];
    int indiceMenor = inicio - 1;

    for (int j = inicio; j < fim; j++)
    {
        if (strcmp(users[j].nome, pivo.nome) < 0)
        {
            indiceMenor++;
            trocar(&users[indiceMenor], &users[j]);
        }
    }

    trocar(&users[indiceMenor + 1], &users[fim]);

    return indiceMenor + 1;
}

void quickSortUsers(Utilizador *users, int inicio, int fim)
{
    if (inicio < fim)
    {
        int indicePivo = particionar(users, inicio, fim);
        quickSortUsers(users, inicio, indicePivo - 1);
        quickSortUsers(users, indicePivo + 1, fim);
    }
}

//-------------------------------Funcões de Validações-------------------------------

int validateMakeReservation(int valor)
{
    bool continuar = true;

    while (continuar)
    {
        if (scanf("%d", &valor) == 0)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(1);
            clear();
            menuUser();
        }
        else
        {
            continuar = false;
        }
    }

    return valor;
}
int validateMenuUser(int valor)
{
    bool continuar = true;

    while (continuar)
    {
        if (scanf("%d", &valor) == 0)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(1);
            clear();
            menuUser();
        }
        else
        {
            continuar = false;
        }
    }

    return valor;
}

int validateMenuLogin(int valor)
{

    bool continuar = true;

    while (continuar)
    {
        if (scanf("%d", &valor) == 0)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(1);
            clear();
            menuLogin();
        }
        else
        {
            continuar = false;
        }
    }

    return valor;
}

int validateMenuAdmin(int valor)
{

    bool continuar = true;

    while (continuar)
    {
        if (scanf("%d", &valor) == 0)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(1);
            clear();
            menuAdmin();
        }
        else
        {
            continuar = false;
        }
    }

    return valor;
}

//-------------------------------Funcões de Menus-------------------------------

void menuLogin()
{
    int escolha;
    bool sair = false;

    do
    {
        signal(SIGINT, handle_sigint);
        clear();
        art();
        printf("\n\n");
        printf("1 - Entrar\n"
               "2 - Registar\n"
               "3 - Sair\n"
               "-> ");
        escolha = validateMenuLogin(escolha);

        switch (escolha)
        {
        case 1:
            autenticarConta();
            break;

        case 2:
            registarConta();
            break;
        case 3:
            sair = true;
            exit(stdout);
            break;

        default:
            printf("Valor inválido. Tente Novamente\n");
            sleep(1);
            clear();
            break;
        }
    } while (sair == false);
}

void menuUser()
{

    int escolha;
    bool sair = false;

    do
    {
        signal(SIGINT, handle_sigint);
        clear();
        art();
        printf("\n\n");
        printf("1 - Iniciar Reserva\n"
               "2 - Consultar Reserva\n"
               "3 - Avalia-nos\n"
               "4 - Sair\n"
               "-> ");
        escolha = validateMenuUser(escolha);

        switch (escolha)
        {
        case 1:
            fazerReserva();
            break;

        case 2:
            consultarReserva();
            break;
        case 3:
            userAvalia(feedback, &countAvaliacao);
            break;
        case 4:
            sair = true;
            break;

        default:
            printf("Valor inválido. Tente Novamente\n");
            sleep(1);
            clear();
            break;
        }
    } while (sair == false);

    clear();
    menuLogin();
}

void menuAdmin()
{

    int escolha;
    bool sair = false;

    do
    {
        signal(SIGINT, handle_sigint);
        clear();
        art();
        printf("\n\n");
        printf("1 - Exibir Histórico de Reservas\n"
               "2 - Listar Utilizadores\n"
               "3 - Limpar Reservas e Avançar Mês\n"
               "4 - Visualizar Avaliações\n"
               "5 - Sair\n"
               "-> ");
        escolha = validateMenuAdmin(escolha);

        switch (escolha)
        {
        case 1:
            exibirHistoricoReservas();
            break;

        case 2:
            listarUtilizadores();
            break;

        case 3:
            limparReservasAvancarMes();
            break;
        case 4:
            visualizarAvaliacoes(feedback, countAvaliacao);
            break;
        case 5:
            clear();
            menuLogin();
            sair = true;
            break;

        default:
            printf("Valor inválido. Tente Novamente\n");
            sleep(1);
            clear();
            break;
        }
    } while (sair == false);

    clear();
    menuLogin();
}

void barraCarregamento(int porcentagem)
{
    int larguraBarra = 50;
    int position = (larguraBarra * porcentagem) / 100;

    printf(BLUE "\r[");
    for (int i = 0; i < larguraBarra; i++)
    {
        if (i < position)
        {
            printf("=");
        }
        else
        {
            printf(" ");
        }
    }

    printf(BLUE "] %d%%" RESET, porcentagem);
    fflush(stdout);
}

void mostrarCarregamento()
{
    printf("\n");

    for (int i = 0; i <= 100; i++)
    {
        barraCarregamento(i);
        usleep(15000);
    }

    printf("\n");

    sleep(1);
}

void lerSenha(char *palavraPasse, int tamanhoMax)
{
    int i = 0;
    char ch;
    bool continuar = true;

#ifdef _WIN32
    while (i < tamanhoMax - 1 && continuar)
    {
        ch = getch();

        if (ch == 13) // 13 = Enter
        {
            continuar = false;
        }
        else if (ch == 8) // 8 = backspace
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            palavraPasse[i] = ch;
            i++;
            printf("*");
        }
    }
#else
    char valor[40];
    scanf("%s", valor);
#endif

    palavraPasse[i] = '\0';
}

bool verificarUserExistente(const char *nome)
{

    FILE *fptr = fopen("utilizadores.txt", "r");
    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return false;
    }
    else
    {
        Utilizador user;

        while (fscanf(fptr, "%s %s", user.nome, user.palavraPasse) == 2)
        {
            if (strcmp(nome, user.nome) == 0)
            {
                return true;
            }
        }

        fclose(fptr);
        return false;
    }
}

void registarConta()
{
    clear();

    Utilizador user;

    FILE *fptr = fopen("utilizadores.txt", "a+");
    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return;
    }
    else
    {
        art();
        printf("\n\n");
        puts("Utilizador: ");
        printf("-> ");
        scanf("%s", user.nome);

        puts("Palavra-passe: ");
        printf("-> ");
        lerSenha(user.palavraPasse, MAX_PASSWORD);

        if (verificarUserExistente(user.nome))
        {
            mostrarCarregamento();
            msgNomeExistente();
            fflush(stdout); // Garante que a mensagem seja exibida imediatamente
            sleep(1);       // Aguarda 1 segundo
        }
        else
        {
            mostrarCarregamento();
            msgSucessoRegistar();
            fprintf(fptr, "%s %s\n", user.nome, user.palavraPasse);
        }
    }

    fclose(fptr);
}

void msgSucessoRegistar()
{
    printf(GREEN "\nConta registrada com sucesso!\n");
    printf(RESET);
    sleep(1);
}

void msgNomeExistente()
{

    printf(RED "\nEsse nome já existe!");
    printf(RESET);
    sleep(1);
}

void autenticarConta()
{
    clear();

    Utilizador user;
    Utilizador userFicheiro;
    bool loginSuccess = false;
    bool lerFicheiro = true;

    FILE *fptr = fopen("utilizadores.txt", "a+");
    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return;
    }
    else
    {
        art();
        printf("\n\n");
        puts("Utilizador: ");
        printf("-> ");
        scanf("%s", user.nome);

        puts("Palavra-passe");
        printf("-> ");
        lerSenha(user.palavraPasse, MAX_PASSWORD);

        if (strcmp(user.nome, "admin") == 0 && strcmp(user.palavraPasse, "admin123") == 0)
        {
            mostrarCarregamento();
            printf(GREEN "\nLogin feito com sucesso!" RESET);
            fflush(stdout);
            sleep(1);
            menuAdmin();
            return;
        }

        while (fscanf(fptr, "%s %s", userFicheiro.nome, userFicheiro.palavraPasse) == 2 && lerFicheiro)
        {
            if (strcmp(user.nome, userFicheiro.nome) == 0 && strcmp(user.palavraPasse, userFicheiro.palavraPasse) == 0)
            {
                loginSuccess = true;
                lerFicheiro = false;
                strcpy(userLogado , user.nome);
            }
        }

        fclose(fptr);

        if (loginSuccess)
        {
            mostrarCarregamento();
            printf(GREEN "\nLogin feito com sucesso!" RESET);
            fflush(stdout);
            sleep(1);
            menuUser();
            return;
        }
        else
        {
            mostrarCarregamento();
            printf(RED "\nLogin inválido! Tente Novamente" RESET);
            fflush(stdout);
            sleep(1);
        }
    }
}

//-------------------------------Funçõs do Cliente-------------------------------

// Funções para fazer reserva
bool verificarReserva(int dia, int quarto)
{
    Reserva reserva;
    FILE *fptr = fopen("reservas.txt", "a+");
    if (fptr == NULL)
    {
        printf("Erro ao abrir o ficheiro\n");
        return false;
    }

    while (fscanf(fptr, "%d %d %d %d %d %d %d %s",
                  &reserva.numContribuinte,
                  &reserva.checkIn, &reserva.checkOut,
                  &reserva.quarto[0], &reserva.quarto[1],
                  &reserva.quarto[2], &reserva.quarto[3], reserva.tipoQuarto) == 8)
    {
        if (dia >= reserva.checkIn && dia <= reserva.checkOut && reserva.quarto[quarto] == 1)
        {
            fclose(fptr);
            return true;
        }
    }

    fclose(fptr);
    return false;
}

void desenharMapaHotel(int checkIn, int checkOut, int *quartosReservados)
{
    printf("Dias: %d até %d\n", checkIn, checkOut);
    for (int i = 0; i < NUM_QUARTOS; i++)
    {
        if (quartosReservados[i] == 1)
        {
            printf(RED);
        }
        else
        {
            printf(GREEN);
        }

        printf("+------------+\n");
        printf("|  Quarto %d |\n", i + 1);
        printf("+------------+\n");
    }
    printf(RESET);
}

void reservarQuarto(int checkIn, int checkOut, int quarto, int numContribuinte, char tipoQuarto[20])
{
    Reserva reserva = {checkIn, checkOut, numContribuinte, "", {0, 0, 0, 0}};

    strcpy(reserva.tipoQuarto, tipoQuarto);

    FILE *fptr = fopen("reservas.txt", "a+");
    if (fptr == NULL)
    {
        printf("Erro ao abrir o ficheiro\n");
        return;
    }

    reserva.quarto[quarto] = 1;

    fprintf(fptr, "%d %d %d %d %d %d %d %s\n",
            reserva.numContribuinte,
            reserva.checkIn, reserva.checkOut,
            reserva.quarto[0], reserva.quarto[1],
            reserva.quarto[2], reserva.quarto[3], reserva.tipoQuarto);
    fclose(fptr);
}

void mostrarCalendario()
{
    int calendario[SEMANAS][DIAS_DA_SEMANA] = {
        {1, 2, 3, 4, 5, 6, 7},
        {8, 9, 10, 11, 12, 13, 14},
        {15, 16, 17, 18, 19, 20, 21},
        {22, 23, 24, 25, 26, 27, 28},
        {29, 30, 31, 0, 0, 0, 0}};

    printf("Calendário - %s / 2024\n", meses[mesAtual]);
    for (int i = 0; i < SEMANAS; i++)
    {
        for (int j = 0; j < DIAS_DA_SEMANA; j++)
        {
            if (calendario[i][j] != 0)
            {
                printf("%2d ", calendario[i][j]);
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
    }
}

void fazerReserva()
{
    signal(SIGINT, handle_sigint);
    clear();

    Reserva reserva;
    bool sair = false;
    int escolhaQuarto;
    int escolhaTipoQuarto;
    int quartosReservados[NUM_QUARTOS] = {0};

    art();
    printf("\n\n");

    printf(YELLOW "Fazer Reserva (0 - sair)\n" RESET);

    mostrarCalendario();
    printf("\n\n");

    do
    {
        printf("Qual dia do Check-In?\n-> ");
        reserva.checkIn = validateMakeReservation(reserva.checkIn);

        if (reserva.checkIn == 0)
        {
            return;
        }

        printf("Qual dia do Check-Out?\n-> ");
        reserva.checkOut = validateMakeReservation(reserva.checkOut);

        if (reserva.checkOut == 0)
        {
            return;
        }

        if (reserva.checkIn >= 32 || reserva.checkIn <= 0 || reserva.checkOut >= 32 || reserva.checkOut <= 0 || reserva.checkIn > reserva.checkOut)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(2);
            clear();
            menuUser();
        }

        sair = true;
    } while (sair == false);

    for (int i = 0; i < NUM_QUARTOS; i++)
    {
        for (int dia = reserva.checkIn; dia <= reserva.checkOut; dia++)
        {
            if (verificarReserva(dia, i))
            {
                quartosReservados[i] = 1; // quarto reservado
            }
        }
    }

    desenharMapaHotel(reserva.checkIn, reserva.checkOut, quartosReservados);

    printf("Escolha o quarto que deseja reservar (1-4)\n"
           "-> ");
    escolhaQuarto = validateMakeReservation(escolhaQuarto) - 1;

    if (escolhaQuarto >= 0 && escolhaQuarto < NUM_QUARTOS && !quartosReservados[escolhaQuarto])
    {
        clear();
        art();
        printf("\n\n");
        printf(YELLOW "Fazer Reserva\n" RESET);

        printf("Insira o número de contribuinte:\n-> ");
        reserva.numContribuinte = validateMakeReservation(reserva.numContribuinte);

        if (reserva.numContribuinte == 0)
        {
            return;
        }

        if (reserva.numContribuinte < 0)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(2);
            clear();
            menuUser();
        }

        printf("\nEscolha o tipo de quarto:\n");
        printf("1 - Vip (5.000€ por noite)\n");
        printf("2 - Clássico (3.000€ por noite)\n");
        printf("3 - Quarto Familiar (4.000€ por noite)\n-> ");
        escolhaTipoQuarto = validateMakeReservation(escolhaTipoQuarto);

        if (reserva.numContribuinte == 0)
        {
            return;
        }

        if (escolhaTipoQuarto < 0)
        {
            printf("Valor inválido. Tente Novamente\n");
            while (getchar() != '\n')
                ;
            sleep(2);
            clear();
            menuUser();
        }

        switch (escolhaTipoQuarto)
        {
        case 1:
            strcpy(reserva.tipoQuarto, "Vip");
            break;
        case 2:
            strcpy(reserva.tipoQuarto, "Clássico");
            break;
        case 3:
            strcpy(reserva.tipoQuarto, "Familiar");
            break;
        default:
            printf("Valor inválido. Tente Novamente\n");
            sleep(1);
            clear();
            return;
        }

        reservarQuarto(reserva.checkIn, reserva.checkOut, escolhaQuarto, reserva.numContribuinte, reserva.tipoQuarto);
        mostrarCarregamento();
        printf(GREEN "Reserva efetuada com sucesso!\n\n" RESET);
        sleep(1);
    }

    else
    {
        mostrarCarregamento();
        printf(RED "Quarto indisponível para o período selecionado.\n" RESET);
        sleep(3);
    }
}

// Funções para avaliar-nos
void userAvalia(Feedback *feedback, int *count)
{

    clear();
    art();
    printf("\n\n");

    Feedback novaAvaliacao;

    printf(GREEN "Avaliações\n" RESET);

    printf("Avalie-nos de 1 a 5: ");
    novaAvaliacao.avaliacao = validateMenuUser(novaAvaliacao.avaliacao);

    if (novaAvaliacao.avaliacao <= 0 || novaAvaliacao.avaliacao >= 6)
    {
        printf("Valor inválido. Tente Novamente\n");
        while (getchar() != '\n')
            ;
        sleep(1);
        clear();
        menuUser();
    }

    strcpy(novaAvaliacao.nomeUser , userLogado);
    feedback[*count] = novaAvaliacao;
    (*count)++;

    mostrarCarregamento();

    clear();
    art();
    printf("\n\n");

    printf(GREEN "Avaliação enviada com sucesso!\n" RESET);
    sleep(2);
}

// Funções para consultar reserva

void mostrarCalendarioConsultado(int checkIn, int checkOut)
{
    int calendario[SEMANAS][DIAS_DA_SEMANA] = {
        {1, 2, 3, 4, 5, 6, 7},
        {8, 9, 10, 11, 12, 13, 14},
        {15, 16, 17, 18, 19, 20, 21},
        {22, 23, 24, 25, 26, 27, 28},
        {29, 30, 31, 0, 0, 0, 0}};

    printf("Calendário - %s / 2024\n", meses[mesAtual]);
    for (int i = 0; i < SEMANAS; i++)
    {
        for (int j = 0; j < DIAS_DA_SEMANA; j++)
        {
            if (calendario[i][j] != 0)
            {
                if (calendario[i][j] == checkIn && calendario[i][j] == checkOut)
                {
                    printf(YELLOW "%2d " RESET, calendario[i][j]);
                }
                else if (calendario[i][j] == checkIn)
                {
                    printf(GREEN "%2d " RESET, calendario[i][j]);
                }
                else if (calendario[i][j] == checkOut)
                {
                    printf(RED "%2d " RESET, calendario[i][j]);
                }
                else
                {
                    printf("%2d ", calendario[i][j]);
                }
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
    }
}

bool verificarReservaUtilizador(int numContribuinte)
{
    FILE *fptr = fopen("reservas.txt", "r");

    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return false;
    }
    else
    {
        Reserva reserva;
        while (fscanf(fptr, "%d %d %d %d %d %d %d %s", &reserva.numContribuinte, &reserva.checkIn, &reserva.checkOut, &reserva.quarto[0], &reserva.quarto[1], &reserva.quarto[2], &reserva.quarto[3], reserva.tipoQuarto) == 8)
        {
            if (reserva.numContribuinte == numContribuinte)
            {
                fclose(fptr);
                return true;
            }
        }

        fclose(fptr);
        return false;
    }
}

void consultarReserva()
{
    clear();

    FILE *fptr = fopen("reservas.txt", "r");
    Reserva reserva;
    bool found = false;
    int numContribuinte;
    art();
    printf("\n\n");

    printf(YELLOW "Consultar Reserva (0 - sair)\n\n" RESET);
    puts("Contribuinte: ");
    printf("-> ");
    numContribuinte = validateMenuUser(numContribuinte);

    if (numContribuinte < 0)
    {
        printf("Valor inválido. Tente Novamente\n");
        while (getchar() != '\n')
            ;
        sleep(1);
        clear();
        menuUser();
    }

    if (numContribuinte == 0)
    {
        return;
    }

    mostrarCarregamento();
    clear();

    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return;
    }

    if (verificarReservaUtilizador(numContribuinte))
    {
        while (fscanf(fptr, "%d %d %d %d %d %d %d %s", &reserva.numContribuinte, &reserva.checkIn, &reserva.checkOut, &reserva.quarto[0], &reserva.quarto[1], &reserva.quarto[2], &reserva.quarto[3], reserva.tipoQuarto) == 8)
        {
            if (reserva.numContribuinte == numContribuinte)
            {
                art();
                printf("\n\n");
                printf(GREEN "Reserva Consultada com sucesso\n\n" RESET);
                mostrarCalendarioConsultado(reserva.checkIn, reserva.checkOut);
                printf("\n");
                printf("\nCheck-In: %d\nCheck-Out: %d\nTipo de Quarto: %s\nNúmero de Contribuinte: %d\n",
                       reserva.checkIn, reserva.checkOut, reserva.tipoQuarto, reserva.numContribuinte);
                
                sleep(8);
                clear();
                
                found = true;
            }
        }

        fclose(fptr);

        if (!found)
        {
            art();
            printf("\n\n");
            printf(RED "Não foram encontradas reservas para o contribuinte %d\n" RESET, numContribuinte);
        }
    }
    else
    {
        art();
        printf("\n\n");
        printf(RED "Não foram encontradas reservas para o contribuinte %d\n" RESET, numContribuinte);
    }

    sleep(3);
    clear();
}

//-------------------------------Funcões do Administrador-------------------------------

void exibirHistoricoReservas()
{
    clear();
    FILE *fptr = fopen("reservas.txt", "a+");

    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return;
    }

    Reserva reserva;
    art();
    printf("\n\n");
    printf("Histórico de Reservas:\n");
    printf("0 - (não-reservado)\n1 - (reservado)\n\n");

    while (fscanf(fptr, "%d %d %d %d %d %d %d %s",
                  &reserva.numContribuinte, &reserva.checkIn, &reserva.checkOut,
                  &reserva.quarto[0], &reserva.quarto[1], &reserva.quarto[2], &reserva.quarto[3], reserva.tipoQuarto) == 8)
    {
        printf("Contribuinte: %d\n"
               "Check-In: %d\n"
               "Check-Out: %d\n"
               "Tipo de Quarto: %s\n"
               "Quartos Reservados: %d %d %d %d\n\n",
               reserva.numContribuinte, reserva.checkIn, reserva.checkOut,
               reserva.tipoQuarto, reserva.quarto[0], reserva.quarto[1],
               reserva.quarto[2], reserva.quarto[3]);
        sleep(3);
    }

    fclose(fptr);
    sleep(5);
}

void listarUtilizadores()
{
    clear();

    FILE *fptr = fopen("utilizadores.txt", "a+");

    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return;
    }

    Utilizador users[200];
    int count = 0;

    while (fscanf(fptr, "%s %s", users[count].nome, users[count].palavraPasse) == 2)
    {
        count++;
    }

    fclose(fptr);

    quickSortUsers(users, 0, count - 1);

    art();
    printf("\n\n");
    printf(YELLOW "Lista de Utilizadores - (Ordem Alfabética) \n\n" RESET);

    printf(GREEN "+----------------------|----------------------+\n");
    printf("| %-20s | %-20s |\n", "Nome", "Palavra-passe");
    printf("+----------------------|----------------------+" RESET);

    // Linhas da tabela
    for (int i = 0; i < count; i++)
    {
        printf("\n| %-20s | ", users[i].nome);

        for (int j = 0; j < strlen(users[i].palavraPasse); j++)
        {
            printf("*");
        }

        // Esse for completa a linha com espaços se a palavra-passe for menor que 20 caracteres
        for (int k = strlen(users[i].palavraPasse); k < 20; k++)
        {
            printf(" ");
        }

        printf(" |");
    }

    // parte de baixo da tabela
    printf(GREEN "\n+----------------------|----------------------+\n" RESET);

    sleep(5);
}

void limparReservasAvancarMes()
{

    clear();
    FILE *fptr = fopen("reservas.txt", "w");

    if (fptr == NULL)
    {
        printf("Erro ao Abrir o ficheiro\n");
        return;
    }

    fclose(fptr);

    mesAtual = (mesAtual + 1) % 12;

    art();
    printf("\n\n");
    printf(GREEN "Reservas Limpas e Mês Avançado para %s" RESET, meses[mesAtual]);
    fflush(stdout);
    sleep(3);
}

void visualizarAvaliacoes(Feedback *feedback, int countAvaliacao)
{
    clear();
    art();
    printf("\n\n");

    if (countAvaliacao == 0)
    {
        printf(RED "Nenhuma avaliação encontrada.\n" RESET);
        return;
    }

    int soma = 0;
    printf(YELLOW "Avaliações dos Utilizadores\n\n" RESET);
    for (int i = 0; i < countAvaliacao; i++)
    {
        printf("%s - Nota: %d\n", feedback[i].nomeUser, feedback[i].avaliacao);
        soma += feedback[i].avaliacao;
    }

    float media = (float)soma / countAvaliacao;

    int estrelas = (int)media;
    puts("\nMédia das avaliações\n");
    for (int i = 0; i < estrelas; i++)
    {
        printf(YELLOW "★" RESET);
    }

    for (int j = estrelas; j < 5; j++)
    {
        printf("☆");
    }

    printf("\n%d estrelas" , (int)media);

    sleep(8);
}

