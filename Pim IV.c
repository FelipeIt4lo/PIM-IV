#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#define LOGIN_DO_USUARIO "felipe"
#define SENHA_DO_USUARIO "felipe123"

static char seu_usuario[20];
static char sua_senha[20];

void login();
void menu();
void registro_do_paciente();
void backMenu();
void checar_grupo_de_risco_paciente(int age, char cep[10]);
void adicionar_arquivo_secretaria_da_saude(int age, char cep[10]);
void lista_do_Paciente();
int calcular_idade(char datanasc[15]);
FILE* openFile(char path[50], char mode);

typedef struct{
    char nome[30];
    char email[30];
    char cpf[10];
    char tel[10];
    char endereco[100];
    char cep[10];
    char datanasc[15];
    char dia_diagnostico[15];
    char comorbidade[50];}

Paciente;

int main()
{
    setlocale(LC_ALL, "Portuguese");
    login();

    return 0;
}
void login(){

    printf("\nOlá, seja bem-vindo ao programa para cadastro de pacientes com covid-19. \n\nPrimeiramente, insira suas informações de login abaixo: \n\nDigite seu Usuário: ");
    scanf("%s", &seu_usuario);
    printf("Digite a sua Senha: ");
    scanf("%s", &sua_senha);

    int comp1 = strncmp(seu_usuario, LOGIN_DO_USUARIO, 6);
    int comp2 = strncmp(sua_senha, SENHA_DO_USUARIO, 9);

    if((comp1 == 0) && (comp2 == 0)){
       menu();
    }else{
        printf("Usuário e/ou senha incorretos, tente novamente.");
        login();
    }
}

void menu(){
    printf("\nO programa conta com 4 opções, por favor, digite um número de 1 a 4 para selecionar as opções listadas abaixo.\n");
    printf("\nOpção 1 - Consultar a Lista dos Pacientes já registrados em nossa base de dados.\n");
    printf("Opção 2 - Cadastrar novo(s) Paciente(s) com covid-19.\n");
    printf("Opção 3 - Enviar lista para a Secretária da saúde.\n");
    printf("Opção 4 - Encerrar o programa\n");

    int option;
    scanf("%d", &option);

    switch(option){
    case 1:
        lista_do_Paciente();
        break;
    case 2:
        registro_do_paciente();
        break;
    case 3:
        printf("Lista enviada a secretária da saúde");
        printf("\nVocê deseja voltar ao menu? S/N\n");
    backMenu();
        break;
    case 4:
        printf("Até depois!");
        exit(0);
        break;

    default:
        printf("\nEssa opção não existe! Por favor, digite outra opção:\n");
        menu();
        break;
    }
}

void registro_do_paciente(){

    FILE *file;
    file = openFile("pacientes_na_base_de_cadastro.txt", 'a');
    if (file == NULL) {
        printf("Desculpe, ocorreu um erro ao cadastrar.");
        exit(0);
    }else {

    Paciente paciente;
    int idade;

    printf("Insira o nome completo do paciente: ");
    fflush(stdin);
    fgets(paciente.nome, 50, stdin);
    fprintf(file,"Nome: %s", paciente.nome);

    printf("Insira o CPF do paciente: ");
    fgets(paciente.cpf, 20, stdin);
    fprintf(file,"CPF: %s", paciente.cpf);

    printf("Insira o e-mail do paciente: ");
    fgets(paciente.email, 50, stdin);
    fprintf(file,"Email: %s", paciente.email);

    printf("Insira o número de contato do paciente: ");
    fgets(paciente.tel, 20, stdin);
    fprintf(file,"Telefone: %s", paciente.tel);

    printf("Insira o endereço completo do paciente: \nEstado, Cidade, bairro, quadra/rua, número da casa/AP.\n");
    fgets(paciente.endereco, 500, stdin);
    fprintf(file,"Endereco: %s", paciente.endereco);

    printf("Insira o CEP do paciente: ");
    fgets(paciente.cep, 20, stdin);
    fprintf(file,"CEP: %s", paciente.cep);

    printf("Insira a data de nascimento do paciente: ");
    fgets(paciente.datanasc, 20, stdin);
    fprintf(file,"Data de nascimento: %s", paciente.datanasc);

    printf("Insira a data do diagnóstico do paciente: ");
    fgets(paciente.dia_diagnostico, 20, stdin);
    fprintf(file,"Data do diagnostico: %s", paciente.dia_diagnostico);

    printf("O paciente possui alguma comorbidade? Sim/Não ");
    char op;
    scanf("%c", &op);
    if(op == 's' || op == 'S'){
        fflush(stdin);
        printf("Qual a comorbidade do paciente? ");
        fgets(paciente.comorbidade, 30, stdin);
        fprintf(file,"Comorbidade: %s\n", paciente.comorbidade);
        idade = calcular_idade(paciente.datanasc);
        adicionar_arquivo_secretaria_da_saude(idade, paciente.cep);
    }else {
        checar_grupo_de_risco_paciente(idade, paciente.cep);
        fprintf(file,"Comorbidade: Nao possui comorbidade\n");
    }
    fclose(file);
    printf("Paciente registrado.\nDeseja voltar ao menu? S/N\n");
    backMenu();
    }
}

void lista_do_Paciente(){

    FILE *file;
    char contents[50];
    file = openFile("pacientes_na_base_de_cadastro.txt", 'r');
    if(file != NULL){

        while(fgets(contents, 50, file) != NULL){
            printf("%s", contents);
        }

    }else{
        printf("O arquivo não foi encontrado.");
    }

    printf("\nVocê deseja voltar ao menu? S/N\n");
    backMenu();
}

void backMenu(){
     char op;
    fflush(stdin);
    scanf("%c", &op);
    if(op == 's' || op == 'S'){
        system("cls");
        menu();
    }else{
        exit(0);
    }

}

FILE* openFile(char path[30], char mode){
    FILE *file;
    switch (mode){
    case 'g' :
        file = fopen(path, "w");
        break;
    case 'r' :
        file = fopen (path, "r");
        break;
    case 'a' :
        file = fopen (path, "a");
        break;
    }
    if (file == NULL){
        printf("Desculpa, não foi possível abrir o arquivo.");
        exit(0);
    }
    return file;
}

int calcular_idade(char datanasc[20]){
    char *ano = strtok(datanasc, "/");
    ano = strtok(NULL, "/");
    ano = strtok(NULL, "/");
    int ano_de_nascimento = atoi(ano);

    int ano_atual;
    time_t data_ano;
    time(&data_ano);
    struct tm *data = localtime(&data_ano);
    ano_atual = (data->tm_year+1900);

    return ano_atual - ano_de_nascimento;
}

void checar_grupo_de_risco_paciente(int idade, char cep[20]){

    if(idade >= 65){
        adicionar_arquivo_secretaria_da_saude(idade, cep);
    }
}

void adicionar_arquivo_secretaria_da_saude(int idade, char cep[20]){

    FILE *file;
    file = openFile("pacientes_grupo_de_risco.txt", 'a');
     if (file == NULL) {
        printf("Erro no cadastro.");
        exit(0);
    }else {
        fprintf(file,"CEP: %s", cep);
        fprintf(file,"Idade: %d\n", idade);
        fclose(file);
    }
}
