#ifndef V3_H
#define V3_H

// Versão sem controle de concorrência para mostrar problemas
void versao_sem_controle(int num_leitores, int num_escritores);

// Variáveis de estatística para versão 3
extern int leituras_realizadas_v3;
extern int escritas_realizadas_v3;

#endif