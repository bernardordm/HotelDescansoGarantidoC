#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 100
#define MAX_ADDRESS_LEN 200
#define MAX_PHONE_LEN 15
#define HEADER_SEPARATOR "==================================================================\n"

typedef struct
{
    int codigo;
    char nome[MAX_NAME_LEN];
    char endereco[MAX_ADDRESS_LEN];
    char telefone[MAX_PHONE_LEN];
} Cliente;

typedef struct
{
    int codigo;
    char nome[MAX_NAME_LEN];
    char telefone[MAX_PHONE_LEN];
    char cargo[MAX_NAME_LEN];
    float salario;
} Funcionario;

typedef struct
{
    int numero;
    int quantidadeHospedes;
    float valorDiaria;
    char status[20];
    int capacidadeMaximaHospedes;
} Quarto;

typedef struct
{
    int codigo;
    char dataEntrada[20];
    char dataSaida[20];
    int quantidadeDiarias;
    int codigoCliente;
    int numeroQuarto;
    int quantidadeHospedes;
    float valorTotal;
} Estadia;

// Variáveis globais para controle dos códigos sequenciais
int proximoCodigoCliente = 1;
int proximoCodigoFuncionario = 1;
int proximoNumeroQuarto = 1;
int proximoCodigoEstadia = 1;

// Função para gerar código sequencial baseado na variável global
int gerarCodigo(const char *tipo)
{
    if (strcmp(tipo, "clientes") == 0)
    {
        return proximoCodigoCliente++;
    }
    else if (strcmp(tipo, "funcionarios") == 0)
    {
        return proximoCodigoFuncionario++;
    }
    else if (strcmp(tipo, "quartos") == 0)
    {
        return proximoNumeroQuarto++;
    }
    else if (strcmp(tipo, "estadias") == 0)
    {
        return proximoCodigoEstadia++;
    }
    else
    {
        return -1; // Tipo inválido
    }
}

// Função para calcular o número de diárias entre duas datas
int calcularDiarias(const char *dataEntrada, const char *dataSaida)
{
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;

    sscanf(dataEntrada, "%d/%d/%d", &diaEntrada, &mesEntrada, &anoEntrada);
    sscanf(dataSaida, "%d/%d/%d", &diaSaida, &mesSaida, &anoSaida);

    struct tm tmEntrada = {0};
    struct tm tmSaida = {0};

    tmEntrada.tm_mday = diaEntrada;
    tmEntrada.tm_mon = mesEntrada - 1;
    tmEntrada.tm_year = anoEntrada - 1900;

    tmSaida.tm_mday = diaSaida;
    tmSaida.tm_mon = mesSaida - 1;
    tmSaida.tm_year = anoSaida - 1900;

    time_t timeEntrada = mktime(&tmEntrada);
    time_t timeSaida = mktime(&tmSaida);

    double diffTime = difftime(timeSaida, timeEntrada);
    int diffDays = (int)(diffTime / (60 * 60 * 24));

    return diffDays;
}


// Funções de cadastro
void cadastrarCliente()
{
    FILE *file = fopen("clientes.txt", "a");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de clientes");
        return;
    }

    Cliente cliente;
    cliente.codigo = gerarCodigo("clientes");

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", cliente.nome);
    printf("Digite o endereco do cliente: ");
    scanf(" %[^\n]", cliente.endereco);
    printf("Digite o telefone do cliente: ");
    scanf(" %[^\n]", cliente.telefone);

    fprintf(file, "Codigo: %d\n", cliente.codigo);
    fprintf(file, "Nome: %s\n", cliente.nome);
    fprintf(file, "Endereco: %s\n", cliente.endereco);
    fprintf(file, "Telefone: %s\n", cliente.telefone);
    fprintf(file, HEADER_SEPARATOR);

    fclose(file);

    printf("Cliente cadastrado com sucesso!\n");
}

void cadastrarFuncionario()
{
    FILE *file = fopen("funcionarios.txt", "a");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de funcionarios");
        return;
    }

    Funcionario funcionario;
    funcionario.codigo = gerarCodigo("funcionarios");

    printf("Digite o nome do funcionario: ");
    scanf(" %[^\n]", funcionario.nome);
    printf("Digite o telefone do funcionario: ");
    scanf(" %[^\n]", funcionario.telefone);
    printf("Digite o cargo do funcionario: ");
    scanf(" %[^\n]", funcionario.cargo);
    printf("Digite o salario do funcionario: ");
    scanf("%f", &funcionario.salario);

    fprintf(file, "Codigo: %d\n", funcionario.codigo);
    fprintf(file, "Nome: %s\n", funcionario.nome);
    fprintf(file, "Telefone: %s\n", funcionario.telefone);
    fprintf(file, "Cargo: %s\n", funcionario.cargo);
    fprintf(file, "Salario: %.2f\n", funcionario.salario);
    fprintf(file, HEADER_SEPARATOR);

    fclose(file);

    printf("Funcionario cadastrado com sucesso!\n");
}

void cadastrarQuarto()
{
    FILE *file = fopen("quartos.txt", "a");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de quartos");
        return;
    }

    Quarto quarto;
    quarto.numero = gerarCodigo("quartos");

    printf("Digite a quantidade de hospedes que o quarto pode acomodar: ");
    scanf("%d", &quarto.capacidadeMaximaHospedes);
    printf("Digite o valor da diaria: ");
    scanf("%f", &quarto.valorDiaria);

    strcpy(quarto.status, "desocupado");

    fprintf(file, "Numero do Quarto: %d\n", quarto.numero);
    fprintf(file, "Quantidade de Hospedes: %d\n", quarto.capacidadeMaximaHospedes);
    fprintf(file, "Valor da Diaria: %.2f\n", quarto.valorDiaria);
    fprintf(file, "Status: %s\n", quarto.status);
    fprintf(file, HEADER_SEPARATOR);

    fclose(file);

    printf("Quarto cadastrado com sucesso!\n");
}

void cadastrarEstadia()
{
    FILE *fileEstadias = fopen("estadias.txt", "a");
    FILE *fileQuartos = fopen("quartos.txt", "r+");
    if (fileEstadias == NULL || fileQuartos == NULL)
    {
        perror("Erro ao abrir arquivos de estadias ou quartos");
        if (fileEstadias != NULL)
            fclose(fileEstadias);
        if (fileQuartos != NULL)
            fclose(fileQuartos);
        return;
    }

    Estadia estadia;
    Quarto quarto;
    int encontrado = 0;

    estadia.codigo = gerarCodigo("estadias");

    printf("Digite a data de entrada (dd/mm/aaaa): ");
    scanf("%s", estadia.dataEntrada);
    printf("Digite a data de saida (dd/mm/aaaa): ");
    scanf("%s", estadia.dataSaida);

    estadia.quantidadeDiarias = calcularDiarias(estadia.dataEntrada, estadia.dataSaida);

    printf("Digite o codigo do cliente: ");
    scanf("%d", &estadia.codigoCliente);

    printf("Digite o numero do quarto: ");
    scanf("%d", &estadia.numeroQuarto);

    printf("Digite o numero de hospedes: ");
    scanf("%d", &estadia.quantidadeHospedes);

    // Verifica se o quarto existe e está desocupado
    while (fscanf(fileQuartos, "Numero do Quarto: %d\nQuantidade de Hospedes: %d\nValor da Diaria: %f\nStatus: %s\n",
                  &quarto.numero, &quarto.capacidadeMaximaHospedes, &quarto.valorDiaria, quarto.status) == 4)
    {
        if (quarto.numero == estadia.numeroQuarto && strcmp(quarto.status, "desocupado") == 0)
        {
            encontrado = 1;
            break;
        }
    }

    if (encontrado && quarto.capacidadeMaximaHospedes >= estadia.quantidadeHospedes)
    {
        estadia.valorTotal = estadia.quantidadeDiarias * quarto.valorDiaria;
        fseek(fileQuartos, -strlen("Status: desocupado\n"), SEEK_CUR);
        fprintf(fileQuartos, "Status: ocupado\n");

        fprintf(fileEstadias, "Codigo: %d\n", estadia.codigo);
        fprintf(fileEstadias, "Data de Entrada: %s\n", estadia.dataEntrada);
        fprintf(fileEstadias, "Data de Saida: %s\n", estadia.dataSaida);
        fprintf(fileEstadias, "Quantidade de Diarias: %d\n", estadia.quantidadeDiarias);
        fprintf(fileEstadias, "Codigo do Cliente: %d\n", estadia.codigoCliente);
        fprintf(fileEstadias, "Numero do Quarto: %d\n", estadia.numeroQuarto);
        fprintf(fileEstadias, "Numero de Hospedes: %d\n", estadia.quantidadeHospedes);
        fprintf(fileEstadias, "Valor Total: %.2f\n", estadia.valorTotal);  // Grava o valor total
        fprintf(fileEstadias, HEADER_SEPARATOR);

        printf("Estadia cadastrada com sucesso!\n");
    }
    else
    {
        printf("Erro: Quarto não encontrado, está ocupado ou a quantidade de hospedes excede a capacidade maxima.\n");
    }

    fclose(fileEstadias);
    fclose(fileQuartos);
}

// Função para dar baixa em uma estadia
// Função para dar baixa em uma estadia
void darBaixaEstadia()
{
    FILE *fileEstadias = fopen("estadias.txt", "r");
    FILE *tempFileEstadias = fopen("temp_estadias.txt", "w");
    FILE *fileQuartos = fopen("quartos.txt", "r+");
    if (fileEstadias == NULL || tempFileEstadias == NULL || fileQuartos == NULL)
    {
        perror("Erro ao abrir arquivos");
        if (fileEstadias != NULL)
            fclose(fileEstadias);
        if (tempFileEstadias != NULL)
            fclose(tempFileEstadias);
        if (fileQuartos != NULL)
            fclose(fileQuartos);
        return;
    }

    int codigoEstadia;
    printf("Digite o codigo da estadia a ser baixada: ");
    scanf("%d", &codigoEstadia);

    Estadia estadia;
    Quarto quarto;
    int encontrado = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fileEstadias))
    {
        sscanf(buffer, "Codigo: %d\n", &estadia.codigo);
        if (estadia.codigo == codigoEstadia)
        {
            encontrado = 1;
            fgets(buffer, sizeof(buffer), fileEstadias); // Pula data de entrada
            fgets(buffer, sizeof(buffer), fileEstadias); // Pula data de saída
            fgets(buffer, sizeof(buffer), fileEstadias); // Pula quantidade de diárias
            fgets(buffer, sizeof(buffer), fileEstadias); // Pula código do cliente
            sscanf(buffer, "Numero do Quarto: %d\n", &estadia.numeroQuarto);
            fgets(buffer, sizeof(buffer), fileEstadias); // Pula número de hóspedes
            fgets(buffer, sizeof(buffer), fileEstadias); // Pula separator

            // Atualiza o status do quarto para "desocupado"
            while (fgets(buffer, sizeof(buffer), fileQuartos))
            {
                sscanf(buffer, "Numero do Quarto: %d\nQuantidade de Hospedes: %d\nValor da Diaria: %f\nStatus: %s\n",
                       &quarto.numero, &quarto.capacidadeMaximaHospedes, &quarto.valorDiaria, quarto.status);
                if (quarto.numero == estadia.numeroQuarto)
                {
                    // Move o ponteiro para o início da linha do status
                    fseek(fileQuartos, -strlen(buffer), SEEK_CUR);
                    // Atualiza o status para "desocupado"
                    fprintf(fileQuartos, "Numero do Quarto: %d\nQuantidade de Hospedes: %d\nValor da Diaria: %.2f\nStatus: desocupado\n",
                            quarto.numero, quarto.capacidadeMaximaHospedes, quarto.valorDiaria);
                    break;
                }
            }
        }
        else
        {
            // Escreve a linha atual no arquivo temporário
            fprintf(tempFileEstadias, "%s", buffer);
        }
    }

    fclose(fileEstadias);
    fclose(tempFileEstadias);
    fclose(fileQuartos);

    if (encontrado)
    {
        remove("estadias.txt");
        rename("temp_estadias.txt", "estadias.txt");
        printf("Estadia dada baixa com sucesso!\n");
    }
    else
    {
        remove("temp_estadias.txt");
        printf("Estadia com o codigo %d nao encontrada!\n", codigoEstadia);
    }
}

// Função para inicializar os arquivos
void inicializarArquivos()
{
    FILE *file;
    file = fopen("clientes.txt", "w");
    if (file != NULL)
        fclose(file);

    file = fopen("funcionarios.txt", "w");
    if (file != NULL)
        fclose(file);

    file = fopen("quartos.txt", "w");
    if (file != NULL)
        fclose(file);

    file = fopen("estadias.txt", "w");
    if (file != NULL)
        fclose(file);
}

void buscarClientePorCodigo(int codigo)
{
    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de clientes");
        return;
    }

    Cliente cliente;
    int encontrado = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file))
    {
        sscanf(buffer, "Codigo: %d\n", &cliente.codigo);
        if (cliente.codigo == codigo)
        {
            encontrado = 1;
            fgets(buffer, sizeof(buffer), file); // Ler a linha Nome
            sscanf(buffer, "Nome: %[^\n]\n", cliente.nome);
            fgets(buffer, sizeof(buffer), file); // Ler a linha Endereco
            sscanf(buffer, "Endereco: %[^\n]\n", cliente.endereco);
            fgets(buffer, sizeof(buffer), file); // Ler a linha Telefone
            sscanf(buffer, "Telefone: %[^\n]\n", cliente.telefone);

            printf("\nDados do Cliente:\n");
            printf("Codigo: %d\n", cliente.codigo);
            printf("Nome: %s\n", cliente.nome);
            printf("Endereco: %s\n", cliente.endereco);
            printf("Telefone: %s\n", cliente.telefone);
            printf(HEADER_SEPARATOR);

            break;
        }
    }

    fclose(file);

    if (!encontrado)
    {
        printf("Cliente com o codigo %d nao encontrado!\n", codigo);
    }
}

void buscarFuncionarioPorCodigo(int codigo)
{
    FILE *file = fopen("funcionarios.txt", "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de funcionarios");
        return;
    }

    Funcionario funcionario;
    int encontrado = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file))
    {
        sscanf(buffer, "Codigo: %d\n", &funcionario.codigo);
        if (funcionario.codigo == codigo)
        {
            encontrado = 1;
            fgets(buffer, sizeof(buffer), file); // Ler a linha Nome
            sscanf(buffer, "Nome: %[^\n]\n", funcionario.nome);
            fgets(buffer, sizeof(buffer), file); // Ler a linha Telefone
            sscanf(buffer, "Telefone: %[^\n]\n", funcionario.telefone);
            fgets(buffer, sizeof(buffer), file); // Ler a linha Cargo
            sscanf(buffer, "Cargo: %[^\n]\n", funcionario.cargo);
            fgets(buffer, sizeof(buffer), file); // Ler a linha Salario
            sscanf(buffer, "Salario: %f\n", &funcionario.salario);

            printf("\nDados do Funcionario:\n");
            printf("Codigo: %d\n", funcionario.codigo);
            printf("Nome: %s\n", funcionario.nome);
            printf("Telefone: %s\n", funcionario.telefone);
            printf("Cargo: %s\n", funcionario.cargo);
            printf("Salario: %.2f\n", funcionario.salario);
            printf(HEADER_SEPARATOR);

            break;
        }
    }

    fclose(file);

    if (!encontrado)
    {
        printf("Funcionario com o codigo %d nao encontrado!\n", codigo);
    }
}

void buscarEstadiasPorCliente(int codigoCliente)
{
    FILE *file = fopen("estadias.txt", "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de estadias");
        return;
    }

    Estadia estadia;
    int encontrado = 0;
    char buffer[256];

    printf("\nEstadias do Cliente (Codigo %d):\n", codigoCliente);
    printf(HEADER_SEPARATOR);

    while (fgets(buffer, sizeof(buffer), file))
    {
        sscanf(buffer, "Codigo: %d\n", &estadia.codigo);
        if (estadia.codigoCliente == codigoCliente)
        {
            encontrado = 1;
            sscanf(buffer, "Data de Entrada: %s\n", estadia.dataEntrada);
            sscanf(buffer, "Data de Saida: %s\n", estadia.dataSaida);
            sscanf(buffer, "Quantidade de Diarias: %d\n", &estadia.quantidadeDiarias);
            sscanf(buffer, "Numero do Quarto: %d\n", &estadia.numeroQuarto);
            sscanf(buffer, "Numero de Hospedes: %d\n", &estadia.quantidadeHospedes);

            printf("Codigo da Estadia: %d\n", estadia.codigo);
            printf("Data de Entrada: %s\n", estadia.dataEntrada);
            printf("Data de Saida: %s\n", estadia.dataSaida);
            printf("Quantidade de Diarias: %d\n", estadia.quantidadeDiarias);
            printf("Numero do Quarto: %d\n", estadia.numeroQuarto);
            printf("Numero de Hospedes: %d\n", estadia.quantidadeHospedes);
            printf(HEADER_SEPARATOR);
        }
    }

    fclose(file);

    if (!encontrado)
    {
        printf("Nenhuma estadia encontrada para o cliente com o codigo %d!\n", codigoCliente);
    }
}

// Função para exibir o menu
void menu()
{
    int opcao;
    do
    {
        printf("\nMenu:\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Cadastrar Funcionario\n");
        printf("3. Cadastrar Quarto\n");
        printf("4. Cadastrar Estadia\n");
        printf("5. Dar Baixa em Estadia\n");
        printf("6. Buscar Cliente por Codigo\n");
        printf("7. Buscar Funcionario por Codigo\n");
        printf("8. Buscar Estadias por Cliente\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            cadastrarCliente();
            break;
        case 2:
            cadastrarFuncionario();
            break;
        case 3:
            cadastrarQuarto();
            break;
        case 4:
            cadastrarEstadia();
            break;
        case 5:
            darBaixaEstadia();
            break;
        case 6:
        {
            int codigo;
            printf("Digite o codigo do cliente: ");
            scanf("%d", &codigo);
            buscarClientePorCodigo(codigo);
        }
        break;
        case 7:
        {
            int codigo;
            printf("Digite o codigo do funcionario: ");
            scanf("%d", &codigo);
            buscarFuncionarioPorCodigo(codigo);
        }
        break;
        case 8:
        {
            int codigo;
            printf("Digite o codigo do cliente: ");
            scanf("%d", &codigo);
            buscarEstadiasPorCliente(codigo);
        }
        break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    } while (opcao != 0);
}

int main()
{
    inicializarArquivos();
    menu();
    return 0;
}
