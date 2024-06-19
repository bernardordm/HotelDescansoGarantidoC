/*
 ============================================================================
 Name        : prj_trabalhoint_hotel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Declaração das structs
struct pessoa {
    char nome[100];
    int codigo;
};

struct cliente {
    char endereco[100];
    char nome[100];
    double telefone;
    int codigo;
};

struct funcionario {
    char cargo[100];
    char nome[100];
    double salario;
    int codigo;
};

struct estadia {
    int codigo_estadia;
    int data_entrada;
    int data_saida;
    int diarias;
};

struct quarto {
    int numero_quarto;
    int quant_pessoas;
    double preco_diaria;
    int status;
};

// Função para cadastrar novo cliente
int cadastrarCliente() {
    // Implementação aqui
    return 0;
}

// Função para cadastrar novo funcionário
int cadastrarFuncionario() {
    // Implementação aqui
    return 0;
}

// Função para cadastrar nova estadia
int cadastrarEstadia() {
    // Implementação aqui
    return 0;
}

// Função para dar baixa em uma estadia
int checkOutEstadia() {
    // Implementação aqui
    return 0;
}

// Função para pesquisar uma pessoa no sistema
int pesquisaPessoas() {
    // Implementação aqui
    return 0;
}

// Função para visualizar as estadias de um cliente
int visualizarEstadias() {
    // Implementação aqui
    return 0;
}

int main(void) {
    // Implementação do menu do usuário
    int opcao_Menu;
    printf("-------HOTEL DESCANSO GARANTIDO-------\n");
    do {
        printf("--------MENU--------\n");
        printf("Escolha uma opção:\n");
        printf("1- Cadastrar um cliente\n2- Cadastrar um funcionario\n3- Cadastrar uma estadia\n4- Check Out da estadia\n5- Pesquisa de cadastro\n6- Visualizar as estadias do cliente\n7- Finalizar a operação\n");
        scanf("%d", &opcao_Menu);

        switch (opcao_Menu) {
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
                checkOutEstadia();
                break;

            case 5:
                pesquisaPessoas();
                break;

            case 6:
                visualizarEstadias();
                break;

            case 7:
                printf("Operação finalizada!\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao_Menu != 7);

    return 0;
}
