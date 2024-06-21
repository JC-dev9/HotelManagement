void handle_sigint(int sig);
void clear();
void art();
int validateMakeReservation(int valor);
int validateMenuUser(int valor);
int validateMenuLogin(int valor);
int validateMenuAdmin(int valor);
void menuLogin();
void menuUser();
void menuAdmin();
void barraCarregamento(int porcentagem);
void mostrarCarregamento();
void lerSenha(char *palavraPasse, int tamanhoMax);
bool verificarUserExistente(const char *nome);
void registarConta();
void msgSucessoRegistar();
void msgNomeExistente();
void autenticarConta();
bool verificarReserva(int dia, int quarto);
void desenharMapaHotel(int checkIn, int checkOut, int *quartosReservados);
void reservarQuarto(int checkIn, int checkOut, int quarto, int numContribuinte, char tipoQuarto[20]);
void mostrarCalendario();
void fazerReserva();
void consultarReserva();

void exibirHistoricoReservas();
void listarUtilizadores();
void limparReservasAvancarMes();

