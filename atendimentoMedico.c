#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXNOME 60
#define MAXCPF 15 
#define MAXHISTORICO 200
#define MAXCRM 10
#define MAXCONSULTAS 25


typedef struct {
    int idPaciente;
    char nome[MAXNOME];
    char cpfPaciente[MAXCPF];
    int idade;
    char historicoMedico[MAXHISTORICO];
} paciente;

typedef struct {
    int idMedico;
    char nome[MAXNOME];
    char crmMedico[MAXCRM];
    char estadoEmissor[3]; 
    char especialidade[MAXNOME];
} medico;

typedef struct {
    int idConsulta;
    int dia, mes, ano;
    int hora, minuto;
    int idPaciente;
    int idMedico;
    int status; 
} consulta;


typedef struct {
    int idConsulta;
    int idPaciente;
    int idMedico;
    int dia, mes, ano;
    int hora;
    char observacoes[MAXHISTORICO];
} historicoConsulta;

// ----------------Função limpar buffer------------------------
    void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------------- Funções de Validação ------------------------

    int validarSomenteDigitos(const char *str, int apenasDigitos) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            if (!apenasDigitos && (str[i] == '.' || str[i] == '-')) {
                continue;
            }
            return 0; 
        }
    }
    return 1;
}

//---------------Cadastrar Medico---------------
    void cadastrarMedico(medico **lista, int *total) {

    *lista = realloc(*lista, (*total + 1) * sizeof(medico));
    if (*lista == NULL) {
        printf("Erro ao alocar memoria para medico.\n");
        return;
    }
    medico *novo = &(*lista)[*total];
    novo->idMedico = *total + 1;

    printf("\n--- Cadastro de Medico ---\n");
    printf("Digite o nome: ");
    
    fgets(novo->nome, MAXNOME, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';
    
    int duplicado;
    do {
        duplicado = 0;

        printf("Digite o CRM (somente numeros): ");
        fgets(novo->crmMedico, MAXCRM, stdin);
        novo->crmMedico[strcspn(novo->crmMedico, "\n")] = '\0';


        if (!validarSomenteDigitos(novo->crmMedico, 1)) {
            printf("\nCRM deve conter apenas numeros. Tente novamente.\n");
            duplicado = 1;
            continue;
        }

        printf("Digite UF (ex: SP): ");
        fgets(novo->estadoEmissor, 3, stdin);
        novo->estadoEmissor[strcspn(novo->estadoEmissor, "\n")] = '\0';
        

        for (int i = 0; novo->estadoEmissor[i] != '\0'; i++) {
            novo->estadoEmissor[i] = toupper((unsigned char)novo->estadoEmissor[i]);
        }
        

        for (int i = 0; i < *total; i++) {
            if (strcmp((*lista)[i].crmMedico, novo->crmMedico) == 0 && 
                strcmp((*lista)[i].estadoEmissor, novo->estadoEmissor) == 0) {
                printf("\nCRM + UF ja cadastrado. Tente novamente.\n\n");
                duplicado = 1;
                break;
            }
        }
    } while (duplicado == 1);
    
    printf("Nome da especialidade: ");
    fgets(novo->especialidade, MAXNOME, stdin);
    novo->especialidade[strcspn(novo->especialidade, "\n")] = '\0';
    
    (*total)++;  
    printf("\nMedico cadastrado com sucesso! ID: %d\n", novo->idMedico);
}

//-----------------Cadastrar Paciente-------------------------
    void cadastrarPaciente(paciente **lista, int *total) {
    *lista = realloc(*lista, (*total + 1) * sizeof(paciente));
    if (*lista == NULL) {
        printf("Erro ao alocar memoria para paciente.\n");
        return;
    }
    paciente *novo = &(*lista)[*total];
    novo->idPaciente = *total + 1;

    printf("\n--- Cadastro de Paciente ---\n");
    printf("Digite o nome: ");
    fgets(novo->nome, MAXNOME, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';
    
    int cpfValido;
    do {
        cpfValido = 1;
        printf("Digite o CPF (somente numeros): ");
        fgets(novo->cpfPaciente, MAXCPF, stdin);
        novo->cpfPaciente[strcspn(novo->cpfPaciente, "\n")] = '\0';
        

        if (!validarSomenteDigitos(novo->cpfPaciente, 1)) {
            printf("\nCPF deve conter apenas numeros. Tente novamente.\n");
            cpfValido = 0;
            continue;
        }


        for (int i = 0; i < *total; i++) {
            if (strcmp((*lista)[i].cpfPaciente, novo->cpfPaciente) == 0) {
                printf("\nCPF ja cadastrado. Tente novamente.\n\n");
                cpfValido = 0;
                break;
            }
        }

    } while (cpfValido == 0);

    do {
        printf("Digite a idade: ");
        if (scanf("%d", &novo->idade) != 1 || novo->idade < 0 || novo->idade > 120) {
            printf("Idade invalida. Digite um valor entre 0 e 120.\n");
            limparBuffer(); 
        } else {
            break;
        }
    } while (1);
    limparBuffer();

    printf("Historico medico (Breve resumo): ");
    fgets(novo->historicoMedico, MAXHISTORICO, stdin);
    novo->historicoMedico[strcspn(novo->historicoMedico, "\n")] = '\0';
    
    (*total)++;
    printf("\nPaciente cadastrado com sucesso! ID: %d\n", novo->idPaciente);
}

//---------------Listar Medico-------------------
    void exibirMedicos(const medico *lista, int total) {
    if (total == 0) {
        printf("\nNenhum medico cadastrado.\n");
        return;
    }
    printf("\n-------------- Lista de Medicos-------------------\n");
    for (int i = 0; i < total; i++) { 
        printf(" ID: %d | Nome: %s | CRM: %s/%s | Especialidade: %s \n", 
               lista[i].idMedico, lista[i].nome, lista[i].crmMedico, lista[i].estadoEmissor, lista[i].especialidade);
    }
}

//--------------------Listar Paciente--------------------
    void exibirPacientes(const paciente *lista, int total) {
    if (total == 0) {
        printf("\nNenhum paciente cadastrado.\n");
        return;
    }
    printf("\n-------------- Lista de Pacientes-------------------\n");
    for (int i = 0; i < total; i++) {
        printf(" ID: %d | Nome: %s | CPF: %s | Idade: %d \n", 
               lista[i].idPaciente, lista[i].nome, lista[i].cpfPaciente, lista[i].idade);

    }
}

// ---------------- Exibir Historico ----------------
    void exibirHistorico(const historicoConsulta *lista, int total) {
    if (total == 0) {
        printf("\nNenhum historico registrado.\n");
        return;
    }

    printf("\n------ HISTORICO DE CONSULTAS ------\n");
    for (int i = 0; i < total; i++) {
        printf("\nRegistro ID: %d (Consulta ID: %d)\n", i + 1, lista[i].idConsulta);
        printf("Paciente ID: %d | Medico ID: %d\n", lista[i].idPaciente, lista[i].idMedico);
        printf("Data: %02d/%02d/%04d | Hora: %02d:00\n", lista[i].dia, lista[i].mes, lista[i].ano, lista[i].hora);
        printf("Observacoes: %s\n", lista[i].observacoes);
    }
}

//---------------Registrar Historico (melhorado) ----------------------------
    void registrarHistorico(historicoConsulta **listaHistorico, int *totalHistorico, const consulta *c, int status) {
    
    *listaHistorico = realloc(*listaHistorico, (*totalHistorico + 1) * sizeof(historicoConsulta));
    if (*listaHistorico == NULL) {
        printf("Erro ao realocar memoria para historico.\n");
        return;
    }
    
    historicoConsulta *novoRegistro = &(*listaHistorico)[*totalHistorico];
    
   
    novoRegistro->idConsulta = c->idConsulta;
    novoRegistro->idPaciente = c->idPaciente;
    novoRegistro->idMedico = c->idMedico;
    novoRegistro->dia = c->dia;
    novoRegistro->mes = c->mes;
    novoRegistro->ano = c->ano;
    novoRegistro->hora = c->hora;
    
  
    if (status == 0) { 
        strcpy(novoRegistro->observacoes, "Consulta agendada.");
    } else if (status == 1) { 
        strcpy(novoRegistro->observacoes, "Consulta cancelada.");
    } else if (status == 2) { 
        strcpy(novoRegistro->observacoes, "Consulta reagendada.");
    } else if (status == 3) { 
        printf("\nDigite observacoes da consulta (Historico clinico): ");
        limparBuffer();
        fgets(novoRegistro->observacoes, MAXHISTORICO, stdin);
        novoRegistro->observacoes[strcspn(novoRegistro->observacoes, "\n")] = '\0';
    } else {
        strcpy(novoRegistro->observacoes, "Status indefinido.");
    }

    (*totalHistorico)++;
    printf("Historico registrado com sucesso!\n");
}

// ---------------Listar Consultas --------------------------
    void exibirConsultas(const consulta *listaConsultas, int totalConsultas) {
    if (totalConsultas == 0) {
        printf("\nAgenda de consultas vazia.\n");
        return;
    }
    printf("\n------Lista de Consultas Agendadas (Status 0: Agendada)------\n");
    for (int i = 0; i < totalConsultas; i++) {

        printf("ID: %d | Paciente ID: %d | Medico ID: %d | Data: %02d/%02d/%04d | Hora: %02d:00 | Status: %d\n", 
               listaConsultas[i].idConsulta, listaConsultas[i].idPaciente, listaConsultas[i].idMedico, 
               listaConsultas[i].dia, listaConsultas[i].mes, listaConsultas[i].ano, listaConsultas[i].hora, listaConsultas[i].status);
    }
}


    void cancelarReagendar(consulta *listaConsultas, int totalConsultas, historicoConsulta **listaHistorico, int *totalHistorico) {
    if (totalConsultas == 0) {
        printf("\nNao ha consultas cadastradas.\n");
        return;
    }

    printf("\n--- Cancelar ou Reagendar Consulta ---\n");
    exibirConsultas(listaConsultas, totalConsultas);

    int idEscolhido;
    printf("\nDigite o ID da consulta: ");
    if (scanf("%d", &idEscolhido) != 1) {
        printf("Entrada invalida.\n");
        limparBuffer();
        return;
    }
    limparBuffer();


    int indice = -1;
    for(int i = 0; i < totalConsultas; i++){
        if(listaConsultas[i].idConsulta == idEscolhido){
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Consulta inexistente.\n");
        return;
    }

    consulta *consultaSelecionada = &listaConsultas[indice];

    int opcao;
    printf("\n1 - Cancelar consulta\n2 - Reagendar consulta\n3 - Marcar como Realizada\nEscolha: ");
    if (scanf("%d", &opcao) != 1) {
        printf("Entrada invalida.\n");
        limparBuffer();
        return;
    }
    limparBuffer();
    
    if (opcao == 1) {
        consultaSelecionada->status = 1; 
        registrarHistorico(listaHistorico, totalHistorico, consultaSelecionada, 1);
        printf("\nConsulta cancelada com sucesso!\n");
    }
    else if (opcao == 2) {

        printf("\nDigite a nova data (DD MM AAAA): ");
        if (scanf("%d %d %d", &consultaSelecionada->dia, &consultaSelecionada->mes, &consultaSelecionada->ano) != 3) {
            printf("Data invalida.\n");
            limparBuffer();
            return;
        }
        limparBuffer();

        do {
            printf("Digite a nova hora (7 a 18): ");
            if (scanf("%d", &consultaSelecionada->hora) != 1 || consultaSelecionada->hora < 7 || consultaSelecionada->hora > 18) {
                printf("Hora invalida! As consultas so podem ser marcadas entre 07h e 18h.\n");
                limparBuffer();
            } else {
                break;
            }
        } while(1);
        limparBuffer();
        
        consultaSelecionada->status = 2; 
        registrarHistorico(listaHistorico, totalHistorico, consultaSelecionada, 2);
        printf("\nConsulta reagendada com sucesso!\n");
    }
    else if (opcao == 3) {
        consultaSelecionada->status = 3;
        registrarHistorico(listaHistorico, totalHistorico, consultaSelecionada, 3);
        printf("\nConsulta marcada como realizada!\n");
    }
    else {
        printf("Opcao invalida.\n");
    }
}


    void buscarRegistro(const paciente *listaPacientes, int totalPacientes, const medico *listaMedicos, int totalMedicos) {
    int opcao;
    printf("\n--- Buscar ---\n");
    printf("1 - Buscar Paciente\n");
    printf("2 - Buscar Medico\n");
    printf("Escolha: ");
    if (scanf("%d", &opcao) != 1) {
        printf("Opcao invalida.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    if (opcao != 1 && opcao != 2) {
        printf("Opcao de busca invalida.\n");
        return;
    }

    char termo[60];
    printf("Digite o nome ou documento para busca: ");
    fgets(termo, 60, stdin);
    termo[strcspn(termo, "\n")] = '\0';
    
   
    char termoLower[60];
    strcpy(termoLower, termo);
    for(int i = 0; termoLower[i]; i++){
        termoLower[i] = tolower((unsigned char)termoLower[i]);
    }

    int encontrado = 0;

    if (opcao == 1) {
        for (int i = 0; i < totalPacientes; i++) {
           
            char nomeLower[MAXNOME];
            strcpy(nomeLower, listaPacientes[i].nome);
            for(int j = 0; nomeLower[j]; j++){
                nomeLower[j] = tolower((unsigned char)nomeLower[j]);
            }

            if (strstr(nomeLower, termoLower) != NULL ||
                strstr(listaPacientes[i].cpfPaciente, termo) != NULL) { // CPF geralmente não tem case-sensitivity

                printf("\nPaciente encontrado:\n");
                printf("ID: %d | Nome: %s | CPF: %s | Idade: %d\n",
                       listaPacientes[i].idPaciente,
                       listaPacientes[i].nome,
                       listaPacientes[i].cpfPaciente,
                       listaPacientes[i].idade);

                encontrado = 1;
            }
        }
    }
    else if (opcao == 2) {
        for (int i = 0; i < totalMedicos; i++) {
           
            char nomeLower[MAXNOME];
            strcpy(nomeLower, listaMedicos[i].nome);
            for(int j = 0; nomeLower[j]; j++){
                nomeLower[j] = tolower((unsigned char)nomeLower[j]);
            }

            if (strstr(nomeLower, termoLower) != NULL ||
                strstr(listaMedicos[i].crmMedico, termo) != NULL) {

                printf("\nMedico encontrado:\n");
                printf("ID: %d | Nome: %s | CRM: %s/%s | Especialidade: %s\n",
                       listaMedicos[i].idMedico,
                       listaMedicos[i].nome,
                       listaMedicos[i].crmMedico,
                       listaMedicos[i].estadoEmissor,
                       listaMedicos[i].especialidade);

                encontrado = 1;
            }
        }
    }

    if (!encontrado) {
        printf("\nNenhum registro encontrado.\n");
    }
}


// --------------- Agendamento de consulta--------------------
    void agendarConsulta(consulta **listaConsultas, const paciente *listaPacientes, int totalPacientes, 
    const medico *listaMedicos, int totalMedicos, int *totalConsultas, historicoConsulta **listaHistorico, int *totalHistorico) {
    if (totalPacientes == 0 || totalMedicos == 0) {
        printf("\nNecessario cadastrar ao menos 1 paciente e 1 medico.\n");
        return;
    } else if (*totalConsultas >= MAXCONSULTAS) {
        printf("\nO limite de %d consultas foi atingido.\n", MAXCONSULTAS);
        return;
    }


    *listaConsultas = realloc(*listaConsultas, (*totalConsultas + 1) * sizeof(consulta));

    if (*listaConsultas == NULL) {
        printf("Erro ao alocar memoria para consulta.\n");
        return;
    }
    
    consulta *nova = &(*listaConsultas)[*totalConsultas];
    nova->idConsulta = *totalConsultas + 1;

    // --- Seleção de Paciente ---
    int idSelecionado;
    do {
        exibirPacientes(listaPacientes, totalPacientes);
        printf("\nDigite o ID do paciente: ");
        if (scanf("%d", &idSelecionado) != 1) {
            printf("Entrada invalida.\n");
            limparBuffer();
            idSelecionado = 0;
            continue;
        }
        limparBuffer();


        if (idSelecionado < 1 || idSelecionado > totalPacientes) {
            printf("ID de paciente invalido.\n");
        }
    } while (idSelecionado < 1 || idSelecionado > totalPacientes);
    nova->idPaciente = idSelecionado;

    // --- Seleção de Médico ---
    do {
        exibirMedicos(listaMedicos, totalMedicos);
        printf("\nDigite o ID do Medico: ");
        if (scanf("%d", &idSelecionado) != 1) {
            printf("Entrada invalida.\n");
            limparBuffer();
            idSelecionado = 0; 
            continue;
        }
        limparBuffer();

        
        if (idSelecionado < 1 || idSelecionado > totalMedicos) {
            printf("ID de medico invalido.\n");
        }
    } while (idSelecionado < 1 || idSelecionado > totalMedicos);
    nova->idMedico = idSelecionado;

    // --- Data ---

    printf("Digite a data (DD MM AAAA): ");
    if (scanf("%d %d %d", &nova->dia, &nova->mes, &nova->ano) != 3) {
        printf("Data invalida. Cancelando agendamento.\n");

        limparBuffer();
        return; 
    }
    limparBuffer();

    // --- Hora ---
    do {
        printf("\nEscolha um horario entre 7 e 18 horas (somente hora cheia): ");
        if (scanf("%d", &nova->hora) != 1 || nova->hora < 7 || nova->hora > 18) {
            printf("Hora invalida! As consultas so podem ser marcadas entre 07h e 18h.\n");
            limparBuffer();
        } else {
            break;
        }
    } while (1);
    limparBuffer();
    
    nova->minuto = 0;
    nova->status = 0;

    (*totalConsultas)++;
    printf("\nConsulta agendada com sucesso! ID: %d\n", nova->idConsulta);


    registrarHistorico(listaHistorico, totalHistorico, nova, 0);
}

// ---------------Menu cadastrar ----------------------------
void menuCadastro(paciente **listaPacientes, int *totalPacientes, medico **listaMedicos, int *totalMedicos) {
    int opcao;
    do {
        printf("\nEscolha uma opcao de cadastro:\n");
        printf("1 - Cadastrar Medico.\n");
        printf("2 - Cadastrar Paciente.\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
             printf("\nOpcao invalida. Tente novamente.\n");
             limparBuffer();
             opcao = 0; 
             continue;
        }
        limparBuffer();
        if (opcao < 1 || opcao > 2) {
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao < 1 || opcao > 2);

    if (opcao == 1) {
        cadastrarMedico(listaMedicos, totalMedicos);
    } else {
        cadastrarPaciente(listaPacientes, totalPacientes);
    }
}

// --------------------Menu Principal---------------------
int menuPrincipal() {
    int menu;
    do {
        printf("\n------------------------------------------------\n");
        printf("                       Menu                     ");
        printf("\n------------------------------------------------\n");
        printf("Digite o numero correspondente a opcao desejada:\n");
        printf("1-Cadastrar (Medico/Paciente)\n");
        printf("2-Agendar consulta\n");
        printf("3-Historico de consultas\n");
        printf("4-Cancelar/reagendar/realizar consulta\n");
        printf("5-Buscar (Medico/Paciente)\n");
        printf("6-Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &menu) != 1) {
            printf("\nOpcao invalida. Tente novamente.\n");
            limparBuffer();
            menu = 0; 
            continue;
        }
        limparBuffer();
        if (menu < 1 || menu > 6) {
            printf("\nOpcao invalida. Tente novamente.\n");
        }
    } while (menu < 1 || menu > 6);
    
    return menu;
}

    int main() {
    int menu = 0;

    paciente *listaPacientes = NULL;
    int totalPacientes = 0;
    medico *listaMedicos = NULL;
    int totalMedicos = 0;
    consulta *listaConsultas = NULL;
    int totalConsultas = 0;
    historicoConsulta *listaHistorico = NULL;
    int totalHistorico = 0;

    while (menu != 6) {
        menu = menuPrincipal();
        
        if (menu == 1) {
            menuCadastro(&listaPacientes, &totalPacientes, &listaMedicos, &totalMedicos);

        }
        else if (menu == 2) {
            agendarConsulta(&listaConsultas, listaPacientes, totalPacientes, listaMedicos, totalMedicos, &totalConsultas, &listaHistorico, &totalHistorico);
            exibirConsultas(listaConsultas, totalConsultas);
        }
        else if (menu == 3) {
            exibirHistorico(listaHistorico, totalHistorico);
        }
        else if (menu == 4) {
            cancelarReagendar(listaConsultas, totalConsultas, &listaHistorico, &totalHistorico);
        }
        else if (menu == 5) {
            buscarRegistro(listaPacientes, totalPacientes, listaMedicos, totalMedicos);
        }
    }

    printf("\nSaindo do programa. Liberando memoria...");
    

    free(listaPacientes);
    free(listaMedicos);
    free(listaConsultas);
    free(listaHistorico); 
    
    printf("\nMemoria liberada. Fim do programa.\n");
    return 0;
}