#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>
#define Try if(setjmp(savebuf) == 0)

typedef struct Task Task;
jmp_buf savebuf;

struct Task{
    int identifier;
    char title[100];
    char description[500];
    int done;
    char date_create[12];
    char expiration_date[12];
    Task *next;
}*last_insert = NULL;

int main_menu(){
    int op;
    printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
    printf("|                                     |\n");
    printf("| BEM VINDO AO GERENCIADOR DE TAREFAS |\n");
    printf("|                                     |\n");
    printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
    printf("\n1 - Cadastrar tarefa.");
    printf("\n2 - Gerenciar tarefa.");
    printf("\n3 - Mostrar tarefas.");
    printf("\n0 - Finalizar.");
    printf("\nInforme sua opcao: ");
    scanf("%d", &op);
    return op;
}

int check_done_task(Task *task){
    if(task->done){
        printf("\nEsta opcao nao e valida pois a tarefa ja foi concluida.");
        printf("\nApenas exclua ou volte para o menu anterior.\n\n");
        sleep(2);
        return 0;
    }else
        return 1;
}

void manage_selected_task(Task *task){
    int op;
    do{
        printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
        printf("| TITULO: %s                  \n",task->title);
        printf("| Criado em %s                              \n", task->date_create);
        printf("| --------------------------------------------------- \n");
        printf("| %s                              \n", task->description);
        printf("|                                                   \n");
        printf("|                                                   \n");
        printf("|                                                   \n");
        printf("|                                                   \n");
        printf("|                                                   \n");
        printf("|                                                   \n");
        printf("|                                                   \n");
        printf("| --------------------------------------------------- \n");
        printf("| Essa tarefa expira em: %s                  |\n", task->expiration_date);
        if(task->done){
            printf("| ------------------------------------------------- |\n");
            printf("|                TAREFA CONCLUIDA                   |\n");
        }
        printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
        
        printf("\n1 - Excluir tarefa.");
        if(!task->done){
            printf("\n2 - Editar titulo.");
            printf("\n3 - Editar descricao.");
            printf("\n4 - Editar data de conclusao.");
            printf("\n5 - Marcar como concluida.");
        }
        printf("\n0 - Voltar.");
        printf("\nOpcao: ");
        scanf("%d", &op);

        switch (op){
            case 1:{ // DELETE TASK
                Task *aux = last_insert;
                while (aux != NULL){
                    if(aux == task){
                        last_insert = aux->next;
                        aux = NULL;
                        printf("\nTAREFA EXCLUIDA COM SUCESSO.\n");
                        printf("\nVOLTANDO PARA O MENU ANTERIOR.\n\n");
                        sleep(2);
                        op = 0;
                        break;

                    }else if(aux->next == task){
                        aux->next = task->next;
                        task = NULL;
                        printf("\nTAREFA EXCLUIDA COM SUCESSO.\n");
                        printf("\nVOLTANDO PARA O MENU ANTERIOR.\n\n");
                        sleep(2);
                        op = 0;
                        break;
                    }
                    aux = aux->next;
                }
                break;

            }case 2:{ // EDIT TITLE
                if(check_done_task(task)){
                    char title_change[100];
                    printf("\nInforme um novo titulo para esta tarefa: ");
                    getchar();
                    gets(title_change);
                    strcpy(task->title, title_change);
                    printf("\nTITULO ALTERADO COM SUCESSO.\n\n");
                    sleep(2);
                }
                break;

            }case 3: // EDIT DESCRIPTION
                if(check_done_task(task)){
                    char description_change[100];
                    printf("\nInforme uma nova descricao para esta tarefa: ");
                    getchar();
                    gets(description_change);
                    strcpy(task->description, description_change);
                    printf("\nDESCRICAO ALTERADA COM SUCESSO.\n\n");
                    sleep(2);
                }
                break;

            case 4:{ // EDIT COMPLETION DATE
                if(check_done_task(task)){
                    int day, month;
                    char year[5], new_date[12];
                    printf("\nInforme o novo dia de conclusao: ");
                    scanf("%d", &day);
                    printf("\nInforme o novo mes de conclusao: ");
                    scanf("%d", &month);
                    printf("\nInforme o novo ano de conclusao: ");
                    getchar();
                    gets(year);

                    Try{
                        check_date(day, month, year);
                        sprintf(new_date, "%d/%d/%s", day, month, year);
                        strcpy(task->expiration_date, new_date);
                        printf("\nESTA TAREFA TEVE A DATA DE CONCLUSAO ALTERADA.\n\n");
                        sleep(2);
                    }else{
                        printf("\nOCORREU UM ERRO NA VALIDACAO DA DATA.\nVERIFIQUE OS DADOS E TENTE NOVAMENTE.\n\n");
                        sleep(2);
                    }
                }
                break;

            }case 5: // MARK AS DONE
                if(check_done_task(task)){
                    printf("\nESTA TAREFA FOI MARCADA COMO CONCLUIDA.\n\n");
                    task->done = 1;
                    sleep(2);
                }
                break;

            case 0: // RETURN
                printf("\nVOLTANDO PARA O MENU ANTERIOR.\n\n");
                sleep(2);
                break;
        
            default:
                printf("\n+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
                printf("|  OPCAO NAO RECONHECIDA PELO SISTEMA |\n");

                if(task->done) printf("| INFORME SE DESEJA EXCLUIR OU VOLTAR |\n");
                else printf("|    INFORME UMA OPCAO ENTRE 0 E 5    |\n");

                printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n\n");
                break;
        }
    }while(op);
}

void read_initial_tasks(){
    FILE *archive;
    char content_archive[500];
    Task *task = (Task*) malloc(sizeof(Task));

    archive = fopen("tasks.txt", "r");

    if(archive == NULL){
        printf("\nFalha na inicializacao do sistema.\n");
        fclose(archive);
        exit(0);

    }else{
        while(fgets(content_archive, 500, archive)){ 
            sscanf(strtok(content_archive, "|"), "%d", &task->identifier); // converting from char to integer
            strcpy(task->title,           strtok(NULL, "|"));
            strcpy(task->description,     strtok(NULL, "|"));
            sscanf(strtok(NULL, "|"), "%d", &task->done); // converting from char to integer
            strcpy(task->date_create,     strtok(NULL, "|"));
            strcpy(task->expiration_date, strtok(NULL, "|"));

            // taking the line break character
            for (int i = 0; i < strlen(task->expiration_date); i++)
                if(task->expiration_date[i] == '\n') task->expiration_date[i] = '\0';

            task->next = last_insert;
            last_insert = task;
            task = NULL;
            task = (Task*) malloc(sizeof(Task));
        }
    }
    fclose(archive);
}

void show_tasks(){
    printf("\n");
    Task *aux = last_insert;
    while (aux != NULL){
        printf("\n| Cod. | Titulo\n");
        printf("---------------------------------------------\n");
        printf("|  %d   | %s\n", aux->identifier, aux->title);
        printf("---------------------------------------------\n");
        aux = aux->next;
    }
    printf("\n");
}

Task *search_task_by_code(int code){
    Task *aux = last_insert, *back=NULL;
    while (aux != NULL){
        if(aux->identifier == code){
            back = aux;
            break;
        }
        aux = aux->next;
    }
    return back;
}

int search_biggest_identifier(){
    Task *aux = last_insert;
    int back=0;
    while(aux != NULL){
        if (aux->identifier > back) back = aux->identifier;
        
        aux = aux->next;
    }
    // add 1 to the value of the largest identifier
    return back + 1;
}

void create_task(int identifier, char title[100], char description[500], char date_create[12], char expiration_date[12]){
    Task *task = (Task*) malloc(sizeof(Task));
        
    task->identifier = identifier;
    strcpy(task->title, title);
    strcpy(task->description, description);
    task->done = 0;
    strcpy(task->date_create, date_create);
    strcpy(task->expiration_date, expiration_date);
        
    task->next = last_insert;
    last_insert = task;
}

void check_date(int day, int month, char year[5]){
    int days_of_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(day < 1 || day > days_of_month[month-1] || month < 1 || month > 12 || strlen(year) != 4)
        longjmp(savebuf, 1);
}

void update_tasks_txt(){
    FILE *archive;
    archive = fopen("tasks.txt", "w");
    Task *aux = last_insert;
    char update[2000];

    if(archive == NULL){
        printf("\nFalha na gravacao das tarefas no arquivo de persistencia tasks.txt.\n");
        fclose(archive);
        exit(0);
    }else{
        while(last_insert != NULL){
            aux = last_insert;
            sprintf(update, "%d|%s|%s|%d|%s|%s%s",  last_insert->identifier, 
                                                    last_insert->title,
                                                    last_insert->description,
                                                    last_insert->done, 
                                                    last_insert->date_create, 
                                                    last_insert->expiration_date, "\n");
            last_insert = aux->next;
            free(aux);
            aux = NULL;
            fputs(update, archive);
        }
        free(last_insert);
        last_insert = NULL;
    }
}

int main(){
    system("cls");
    int op;
    
    read_initial_tasks();

    do{
        op = main_menu();
        switch (op){
            case 1:{ // REGISTER TASK
                int identifier, day, month;
                char title[500], description[500], date_create[12], expiration_date[12], year[5];

                struct tm *date;
                time_t seconds;
                time(&seconds);
                date = localtime(&seconds);

                if(last_insert) identifier = search_biggest_identifier();
                else identifier = 1;

                printf("\nInforme o titulo da tarefa: ");
                getchar();
                gets(title);

                printf("\nInforme a descricao da tarefa: ");
                gets(description);

                sprintf(date_create, "%d/%d/%d", date->tm_mday, date->tm_mon+1, date->tm_year+1900);

                printf("\nInforme o dia para a conclusao: ");
                scanf("%d", &day);
                printf("\nInforme o mes para a conclusao: ");
                scanf("%d", &month);
                printf("\nInforme o ano para a conclusao: ");
                getchar();
                gets(year);
                
                Try{
                    check_date(day, month, year);
                    sprintf(expiration_date, "%d/%d/%s", day, month, year);
                    create_task(identifier, title, description, date_create, expiration_date);
                    printf("\nTAREFA CADASTRADA COM SUCESSO.\n\n");
                    sleep(1);
                }else{
                    printf("\nOCORREU UM ERRO NA VALIDACAO DA DATA.\nVERIFIQUE OS DADOS E TENTE NOVAMENTE.\n\n");
                    sleep(2);
                }
                break;

            }case 2:{ // MANAGE TASK
                if(last_insert!=NULL){
                    int code=0;
                    Task * task_selected;
                    show_tasks();
                    
                    printf("Informe o codigo da tarefa para seleciona-la: ");
                    scanf("%d", &code);

                    task_selected = search_task_by_code(code);

                    if(task_selected){
                        printf("\n");
                        manage_selected_task(task_selected);
                    }else
                        printf("\n\nTAREFA NAO ENCONTRADA.\n\n");
                }else
                    printf("\nNAO EXISTEM TAREFAS NO SISTEMA.\n\n");

                break;

            }case 3: // SHOW TASKS
                if(last_insert!=NULL)
                    show_tasks();
                else
                    printf("\nNAO EXISTEM TAREFAS NO SISTEMA.\n\n");
                break;

            case 0: // EXIT THE SYSTEM
                printf("\n+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
                printf("|          SAINDO DO SISTEMA          |\n");
                printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n\n");
                update_tasks_txt();
                sleep(2);
                break;

            default:
                printf("\n+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n");
                printf("|  OPCAO NAO RECONHECIDA PELO SISTEMA |\n");
                printf("|    INFORME UMA OPCAO ENTRE 0 E 3    |\n");
                printf("+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+\n\n");
                break;
        }
    }while(op);
}