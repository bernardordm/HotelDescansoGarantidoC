#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição das constantes para os tamanhos dos campos
#define MAX_NAME_LENGTH 100
#define MAX_ADDRESS_LENGTH 200
#define MAX_PHONE_LENGTH 20
#define MAX_CARGO_LENGTH 50

// Definição dos nomes dos arquivos para armazenamento dos dados
#define FILENAME_CLIENTES "clientes.txt"
#define FILENAME_FUNCIONARIOS "funcionarios.txt"
#define FILENAME_ESTADIAS "estadias.txt"
#define FILENAME_QUARTOS "quartos.txt"

// Definição das estruturas para Cliente, Funcionario, Estadia e Quarto
typedef struct
{
    int codigo;
    char nome[MAX_NAME_LENGTH];
    char endereco[MAX_ADDRESS_LENGTH];
    char telefone[MAX_PHONE_LENGTH];
} Cliente;

typedef struct
{
    int codigo;
    char nome[MAX_NAME_LENGTH];
    char telefone[MAX_PHONE_LENGTH];
    char cargo[MAX_CARGO_LENGTH];
    float salario;
} Funcionario;

typedef struct
{
    int codigo;
    char dataEntrada[11]; // Formato YYYY-MM-DD
    char dataSaida[11];   // Formato YYYY-MM-DD
    int quantidadeDiarias;
    int codigoCliente;
    int numeroQuarto;
} Estadia;

typedef struct
{
    int numeroQuarto;
    int quantidadeHospedes;
    float valorDiaria;
    char status[10]; // "ocupado" ou "desocupado"
} Quarto;

// Função para salvar um cliente no arquivo
void salvarCliente(Cliente cliente)
{
    FILE *file = fopen(FILENAME_CLIENTES, "a");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }
    fprintf(file, "%d;%s;%s;%s\n", cliente.codigo, cliente.nome, cliente.endereco, cliente.telefone);
    fclose(file);
}

// Função para salvar um funcionário no arquivo
void salvarFuncionario(Funcionario funcionario)
{
    FILE *file = fopen(FILENAME_FUNCIONARIOS, "a");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }
    fprintf(file, "%d;%s;%s;%s;%.2f\n", funcionario.codigo, funcionario.nome, funcionario.telefone, funcionario.cargo, funcionario.salario);
    fclose(file);
}

// Função para salvar uma estadia no arquivo
void salvarEstadia(Estadia estadia)
{
    FILE *file = fopen(FILENAME_ESTADIAS, "a");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de estadias.\n");
        return;
    }
    fprintf(file, "%d;%s;%s;%d;%d;%d\n", estadia.codigo, estadia.dataEntrada, estadia.dataSaida, estadia.quantidadeDiarias, estadia.codigoCliente, estadia.numeroQuarto);
    fclose(file);
}

// Função para salvar um quarto no arquivo
void salvarQuarto(Quarto quarto)
{
    FILE *file = fopen(FILENAME_QUARTOS, "a");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de quartos.\n");
        return;
    }
    fprintf(file, "%d;%d;%.2f;%s\n", quarto.numeroQuarto, quarto.quantidadeHospedes, quarto.valorDiaria, quarto.status);
    fclose(file);
}

// Função para gerar um novo código baseado no último código salvo no arquivo
int gerarCodigo(char *filename)
{
    FILE *file = fopen(filename, "r");
    int codigo = 1;
    if (file != NULL)
    {
        char linha[256];
        while (fgets(linha, sizeof(linha), file))
        {
            int ultimoCodigo;
            sscanf(linha, "%d", &ultimoCodigo);
            if (ultimoCodigo >= codigo)
            {
                codigo = ultimoCodigo + 1;
            }
        }
        fclose(file);
    }
    return codigo;
}

// Função para cadastrar um novo cliente
void cadastrarCliente()
{
    Cliente cliente;
    cliente.codigo = gerarCodigo(FILENAME_CLIENTES);
    printf("Nome: ");
    fgets(cliente.nome, MAX_NAME_LENGTH, stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = 0;
    printf("Endereço: ");
    fgets(cliente.endereco, MAX_ADDRESS_LENGTH, stdin);
    cliente.endereco[strcspn(cliente.endereco, "\n")] = 0;
    printf("Telefone: ");
    fgets(cliente.telefone, MAX_PHONE_LENGTH, stdin);
    cliente.telefone[strcspn(cliente.telefone, "\n")] = 0;

    salvarCliente(cliente);
    printf("Cliente cadastrado com sucesso!\n");
}

// Função para cadastrar um novo funcionário
void cadastrarFuncionario()
{
    Funcionario funcionario;
    funcionario.codigo = gerarCodigo(FILENAME_FUNCIONARIOS);
    printf("Nome: ");
    fgets(funcionario.nome, MAX_NAME_LENGTH, stdin);
    funcionario.nome[strcspn(funcionario.nome, "\n")] = 0;
    printf("Telefone: ");
    fgets(funcionario.telefone, MAX_PHONE_LENGTH, stdin);
    funcionario.telefone[strcspn(funcionario.telefone, "\n")] = 0;
    printf("Cargo: ");
    fgets(funcionario.cargo, MAX_CARGO_LENGTH, stdin);
    funcionario.cargo[strcspn(funcionario.cargo, "\n")] = 0;
    printf("Salário: ");
    scanf("%f", &funcionario.salario);
    getchar();

    salvarFuncionario(funcionario);
    printf("Funcionário cadastrado com sucesso!\n");
}

// Função auxiliar para converter uma string de data no formato "YYYY-MM-DD" para a estrutura tm
void str_to_tm(char *data, struct tm *tm)
{
    sscanf(data, "%d-%d-%d", &tm->tm_year, &tm->tm_mon, &tm->tm_mday);
    tm->tm_year -= 1900; // Ajusta o ano
    tm->tm_mon -= 1;     // Ajusta o mês
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1;
}

// Função para calcular a quantidade de diárias entre duas datas
int calcularQuantidadeDiarias(char *dataEntrada, char *dataSaida)
{
    struct tm tmEntrada = {0}, tmSaida = {0};
    time_t tEntrada, tSaida;
    str_to_tm(dataEntrada, &tmEntrada);
    str_to_tm(dataSaida, &tmSaida);
    tEntrada = mktime(&tmEntrada);
    tSaida = mktime(&tmSaida);
    return (int)difftime(tSaida, tEntrada) / (60 * 60 * 24);
}

// Função para encontrar um quarto disponível para uma determinada quantidade de hóspedes
int encontrarQuartoDisponivel(int quantidadeHospedes)
{
    FILE *file = fopen(FILENAME_QUARTOS, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de quartos.\n");
        return -1;
    }

    Quarto quarto;
    while (fscanf(file, "%d;%d;%f;%s\n", &quarto.numeroQuarto, &quarto.quantidadeHospedes, &quarto.valorDiaria, quarto.status) != EOF)
    {
        if (quarto.quantidadeHospedes >= quantidadeHospedes && strcmp(quarto.status, "desocupado") == 0)
        {
            fclose(file);
            return quarto.numeroQuarto;
        }
    }
    fclose(file);
    return -1;
}

// Função para atualizar o status de um quarto (ocupado ou desocupado)
void atualizarStatusQuarto(int numeroQuarto, const char *status)
{
    FILE *file = fopen(FILENAME_QUARTOS, "r+");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de quartos.\n");
        return;
    }

    Quarto quarto;
    long pos;
    while ((pos = ftell(file)) != -1 && fscanf(file, "%d;%d;%f;%s\n", &quarto.numeroQuarto, &quarto.quantidadeHospedes, &quarto.valorDiaria, quarto.status) != EOF)
    {
        if (quarto.numeroQuarto == numeroQuarto)
        {
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%d;%d;%.2f;%s\n", quarto.numeroQuarto, quarto.quantidadeHospedes, quarto.valorDiaria, status);
            break;
        }
    }
    fclose(file);
}

// Função para cadastrar um novo quarto
void cadastrarQuarto()
{
    Quarto quarto;
    printf("Número do Quarto: ");
    scanf("%d", &quarto.numeroQuarto);
    getchar();
    printf("Quantidade de Hóspedes: ");
    scanf("%d", &quarto.quantidadeHospedes);
    getchar();
    printf("Valor da Diária: ");
    scanf("%f", &quarto.valorDiaria);
    getchar();
    strcpy(quarto.status, "desocupado");

    salvarQuarto(quarto);
    printf("Quarto cadastrado com sucesso!\n");
}

// Função para cadastrar uma nova estadia
void cadastrarEstadia()
{
    Estadia estadia;
    estadia.codigo = gerarCodigo(FILENAME_ESTADIAS);

    printf("Código do Cliente: ");
    scanf("%d", &estadia.codigoCliente);
    getchar();

    printf("Quantidade de Hóspedes: ");
    int quantidadeHospedes;
    scanf("%d", &quantidadeHospedes);
    getchar();

    printf("Data de Entrada (YYYY-MM-DD): ");
    fgets(estadia.dataEntrada, 11, stdin);
    estadia.dataEntrada[strcspn(estadia.dataEntrada, "\n")] = 0;

    printf("Data de Saída (YYYY-MM-DD): ");
    fgets(estadia.dataSaida, 11, stdin);
    estadia.dataSaida[strcspn(estadia.dataSaida, "\n")] = 0;

    estadia.quantidadeDiarias = calcularQuantidadeDiarias(estadia.dataEntrada, estadia.dataSaida);

    int quartoDisponivel = encontrarQuartoDisponivel(quantidadeHospedes);
    if (quartoDisponivel == -1)
    {
        printf("Nenhum quarto disponível para a quantidade de hóspedes desejada.\n");
        return;
    }

    estadia.numeroQuarto = quartoDisponivel;
    salvarEstadia(estadia);
    atualizarStatusQuarto(estadia.numeroQuarto, "ocupado");

    printf("Estadia cadastrada com sucesso!\n");
}

int main()
{
    int opcao;
    do
    {
        printf("\nMenu:\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Cadastrar Funcionario\n");
        printf("3. Cadastrar Estadia\n");
        printf("4. Cadastrar Quarto\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao)
        {
        case 1:
            cadastrarCliente();
            break;
        case 2:
            cadastrarFuncionario();
            break;
        case 3:
            cadastrarEstadia();
            break;
        case 4:
            cadastrarQuarto();
            break;
        case 5:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
