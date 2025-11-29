#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXNOME 60
#define MAXCPF 12
#define MAXHISTORICO 200
#define MAXCRM 10
#define MAXCONSULTAS 25
//
typedef struct{
    int idPaciente;
    char nome [MAXNOME];
    char cpfPaciente [MAXCPF];
    int idade;
    char historicoMedico [MAXHISTORICO];
}paciente;

typedef struct{
    int idMedico;
    char nome [MAXNOME];
    char crmMedico [MAXCRM];
    char estadoEmissor [3];
    char especialidade [MAXNOME];
}medico;

typedef struct{
    int idConsulta;
    int dia, mes, ano;
    int hora, minuto;
    int idPaciente;
    int idMedico;
}consulta;

// Estrutura historico de consultas
typedef struct{
    
} historicoConsulta;

// ----------------Função limpar buffer------------------------
void limparBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}
//---------------Cadastrar Medico---------------
void cadastrarMedico(medico **lista, int *total){
    *lista = realloc(*lista, (*total + 1) *sizeof(medico));
    if(*lista == NULL){
        printf("Erro ao alocar memoria.\n");
    }
    medico *novo = &(*lista)[*total];
    novo->idMedico = *total + 1;
    printf("Digite o nome: ");
    fgets(novo->nome, MAXNOME, stdin);
    novo->nome [strcspn(novo->nome, "\n")] = '\0';
    int duplicado;
    do{
        duplicado = 0;

        printf("Digite o CRM (somente numeros): ");
        fgets(novo->crmMedico, MAXCRM, stdin);
        novo->crmMedico[strcspn(novo->crmMedico, "\n")] = '\0';
        printf("Digite UF: ");
        fgets(novo->estadoEmissor, 3, stdin);
        novo->estadoEmissor [strcspn(novo->estadoEmissor, "\n")] = '\0';
        for(int i = 0; novo->estadoEmissor[i] != '\0'; i++){
            novo->estadoEmissor[i] = toupper(novo->estadoEmissor[i]);
        }
        for(int i = 0; i< *total; i++){
            if(strcmp((*lista)[i].crmMedico, novo->crmMedico) == 0 && strcmp((*lista)[i].estadoEmissor, novo->estadoEmissor) == 0){
                printf("\n CRM + UF ja cadastrdo. Tente novamente.\n\n");
                duplicado = 1;
                break;
            }
        }
    }while(duplicado  == 1);
    limparBuffer();
    printf("Nome da especialidade: ");
    fgets(novo->especialidade, MAXNOME, stdin);
    novo->especialidade[strcspn(novo->especialidade, "\n")] ='\0';
    (*total)++;  
    printf("\nMedico cadastrado com sucesso!\n");
}
//-----------------Cadastrar Paciente-------------------------
void cadastrarPaciente(paciente **lista, int *total){
    *lista = realloc(*lista, (*total + 1) *sizeof(paciente));
    if(*lista == NULL){
        printf("Erro ao alocar memoria.\n");
    }
    paciente *novo = &(*lista)[*total];
    novo->idPaciente = *total + 1;
    printf("Digite o nome: ");
    fgets(novo->nome, MAXNOME,stdin) ;
    novo->nome [strcspn(novo->nome, "\n")] = '\0';
    printf("Digite o cpf: ");
    fgets(novo->cpfPaciente, MAXCPF, stdin);
    novo->cpfPaciente[strcspn(novo->cpfPaciente,"\n")] = '\0';
    printf("Digite a idade: ");
    scanf("%d",&novo->idade);
    limparBuffer();
    printf("Historico medico:");
    fgets(novo->historicoMedico, MAXHISTORICO, stdin);
    novo->historicoMedico[strcspn(novo->historicoMedico, "\n")] = '\0';
    (*total)++;
    printf("\nPaciente cadastrado com sucesso!\n");
}

//--------------------------Listar Medico--------------------
void exibirMedicos(medico *lista, int total){
    if( total == 0){
        printf("Nenhum medico cadastrado.\n");
        return;
    }
    printf("\n-------------- Lista de medicos-------------------\n");
    for(int i = 0; i<total; i++){ 
    printf(" ID: %d | Nome: %s | CRM: %s/%s | Especialidade: %s \n", lista[i].idMedico, lista[i].nome, lista[i].crmMedico, lista[i].estadoEmissor, lista[i].especialidade);
    }
}
//--------------------------Listar Paciente--------------------
void exibirPacientes(paciente *lista, int total){
    if(total == 0){
        printf("\nNenhum paciente cadastrado.\n");
        return;
    }
    printf("\n-------------- Lista de pacientes-------------------\n");
    for( int i = 0; i <total; i++){
        printf(" ID: %d | Nome: %s | CPF: %s | Idade: %d | Historico Medico: %s \n", lista[i].idPaciente, lista[i].nome, lista[i].cpfPaciente, lista[i].idade, lista[i].historicoMedico);
    }
}

// --------------- Agendamento de consulta--------------------
void agendarConsulta(consulta **listaConsultas, paciente *listaPacientes,int totalPacientes, medico *listaMedico,int totalMedicos, int *totalConsultas){
    if(totalPacientes == 0 || totalMedicos == 0){
        printf("Necessario cadastrar ao menos 1 paciente e 1 medico.\n");
        return;
    }else if(*totalConsultas >= MAXCONSULTAS){
        printf("\nNao ha vagas disponiveis.\n");
        return;
    }

    *listaConsultas = realloc(*listaConsultas, (*totalConsultas + 1) * sizeof(consulta));

    if(*listaConsultas == NULL){
        printf("Erro ao alocar memoria.\n");
        return;
    }
    
    consulta *nova = &(*listaConsultas)[*totalConsultas];
    nova->idConsulta = *totalConsultas + 1;
    //Lista de pacientes
    exibirPacientes(listaPacientes,totalPacientes);
    printf("\nDigite o ID do paciente: ");
    scanf("%d", &nova->idPaciente);
    limparBuffer();
    //Lista de medicos
    exibirMedicos(listaMedico,totalMedicos);
    printf("\nDigite o ID do Medico: ");
    scanf("%d", &nova->idMedico);
    limparBuffer();

    printf("Digite a data (DD MM AAAA): ");
    scanf("%d %d %d", &nova->dia, &nova->mes, &nova->ano);
    limparBuffer();
    
    printf("Digite a hora (7 a 18)");
    scanf("%de", &nova->hora);
    limparBuffer();
    nova->minuto = 0;
    (*totalConsultas)++;
    printf("\nConsulta agendada!\n");
}
// ---------------Listar Consultas --------------------------
void exibirConsultas(consulta *listaConsultas, int totalConsultas){
    if(totalConsultas == 0){
        printf("\nAgenda de consultas vazia.");
        return;
    }
    printf("\nLista de consultas\n ");
    
}

// ---------------Menu cadastrar ----------------------------
void menuCadastro(paciente **listaPacientes, int *totalPacientes, medico **listaMedicos, int *totalMedicos){
    int opcao;
    do{
    printf("\nEscolha uma opcao de cadastro:\n");
    printf("1 - Cadastrar Medico.\n");
    printf("2 - Cadastrar Paciente.\n");
    scanf("%d", &opcao);
    limparBuffer();
    if(opcao < 1 || opcao > 2){
        printf("Invalido. Tente novamente.");
    }
    }while(opcao<1 || opcao>2);

    if(opcao == 1){
        cadastrarMedico(listaMedicos, totalMedicos);
    }
    else{
        cadastrarPaciente(listaPacientes, totalPacientes);
    }
        
}

// --------------------Menu Principal---------------------
int menuPrincipal(int menu ){
    do{
        printf("\n------------------------------------------------\n");
        printf("                       Menu                     ");
        printf("\n------------------------------------------------\n");
        printf("Digite o numero correspondente a opcao desejada:\n");
        printf("1-Cadastrar\n");
        printf("2-Agendar consulta\n");
        printf("3-Historico de consultas\n");
        printf("4-Cancelar/reagendar consulta\n");
        printf("5-Buscar\n");
        printf("6-Sair\n");
        scanf("%d", &menu);
        limparBuffer();
        if(menu < 1 || menu > 6){
            printf("\nOpcao invalida. Tente novamente.\n");
        };
    }while(menu < 1 || menu > 6);
    
    return menu;
}

int main(){
    int menu = 0;
    paciente *listaPacientes = NULL;
    int totalPacientes = 0;
    medico *listaMedicos = NULL;
    int totalMedicos = 0;
    consulta *listaConsultas = NULL;
    int totalConsultas = 0;

    
    while(menu != 6){
        menu = menuPrincipal(menu);
        if(menu == 1){
            menuCadastro(&listaPacientes, &totalPacientes, &listaMedicos, &totalMedicos);
            exibirMedicos(listaMedicos,totalMedicos);
            exibirPacientes(listaPacientes, totalPacientes);

        }
        else if( menu == 2){
            agendarConsulta(&listaConsultas, listaPacientes, totalPacientes, listaMedicos, totalMedicos, &totalConsultas);
            exibirConsultas(listaConsultas, totalConsultas);
        }
    }
    printf("\nSaindo do programa.");
    free(listaPacientes);
    free(listaMedicos);
    free(listaConsultas);
    return 0;
}