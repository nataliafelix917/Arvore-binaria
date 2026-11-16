/*
    AED - Projeto 01
    Sistema de cadastro com arvore binaria de busca
    Tema: controle de estoque de um mercadinho
*/

#include <stdio.h>
#include <stdlib.h>

#define NOME_ARQUIVO "estoque.csv"

typedef struct {
    int codigo;   /* chave */
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct No {
    Produto produto;
    struct No* esquerda;
    struct No* direita;
} No;

No* criarNo(Produto p)
{
    No* novo = (No*) malloc(sizeof(No));
    if (novo == NULL) {
        printf("erro: faltou memoria para criar o no\n");
        exit(1);
    }
    novo->produto = p;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/* menor vai para a esquerda, maior para a direita, repetido nao insere */
No* inserir(No* raiz, Produto p)
{
    if (raiz == NULL) {
        return criarNo(p);
    }
    if (p.codigo < raiz->produto.codigo) {
        raiz->esquerda = inserir(raiz->esquerda, p);
    } else if (p.codigo > raiz->produto.codigo) {
        raiz->direita = inserir(raiz->direita, p);
    }
    return raiz;
}

No* buscar(No* raiz, int codigo)
{
    if (raiz == NULL) {
        return NULL;
    }
    if (codigo == raiz->produto.codigo) {
        return raiz;
    }
    if (codigo < raiz->produto.codigo) {
        return buscar(raiz->esquerda, codigo);
    }
    return buscar(raiz->direita, codigo);
}

void mostrarProduto(Produto p)
{
    printf("%6d  %-30s  R$ %8.2f  %5d\n", p.codigo, p.nome, p.preco, p.quantidade);
}

/* em ordem: sai em ordem crescente de codigo */
void listarEmOrdem(No* raiz)
{
    if (raiz != NULL) {
        listarEmOrdem(raiz->esquerda);
        mostrarProduto(raiz->produto);
        listarEmOrdem(raiz->direita);
    }
}

/* pre-ordem com recuo por nivel. lado: R = raiz, E = esquerda, D = direita */
void exibirArvore(No* raiz, int nivel, char lado)
{
    int i;
    if (raiz != NULL) {
        for (i = 0; i < nivel; i++) {
            printf("     ");
        }
        printf("%c: %d\n", lado, raiz->produto.codigo);
        exibirArvore(raiz->esquerda, nivel + 1, 'E');
        exibirArvore(raiz->direita, nivel + 1, 'D');
    }
}

/* pos-ordem: libera os filhos antes do pai */
void liberarArvore(No* raiz)
{
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

/* grava em pre-ordem para a arvore voltar com o mesmo formato na leitura
   (em ordem o arquivo ficaria crescente e a arvore viraria uma lista) */
void gravarPreOrdem(No* raiz, FILE* arquivo)
{
    if (raiz != NULL) {
        fprintf(arquivo, "%d;%s;%.2f;%d\n", raiz->produto.codigo, raiz->produto.nome,
                raiz->produto.preco, raiz->produto.quantidade);
        gravarPreOrdem(raiz->esquerda, arquivo);
        gravarPreOrdem(raiz->direita, arquivo);
    }
}

void salvarArquivo(No* raiz)
{
    FILE* arquivo = fopen(NOME_ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("erro: nao foi possivel abrir o arquivo\n");
        return;
    }
    gravarPreOrdem(raiz, arquivo);
    fclose(arquivo);
    printf("Dados gravados em %s.\n", NOME_ARQUIVO);
}

/* se o arquivo nao existe a arvore comeca vazia */
No* carregarArquivo(No* raiz)
{
    Produto p;
    FILE* arquivo = fopen(NOME_ARQUIVO, "r");

    if (arquivo != NULL) {
        while (fscanf(arquivo, "%d;%49[^;];%f;%d\n", &p.codigo, p.nome, &p.preco, &p.quantidade) == 4) {
            raiz = inserir(raiz, p);
        }
        fclose(arquivo);
    }
    return raiz;
}

No* cadastrarProduto(No* raiz)
{
    Produto p;

    printf("Codigo: ");
    scanf("%d", &p.codigo);
    if (buscar(raiz, p.codigo) != NULL) {
        printf("Ja existe um produto com o codigo %d.\n", p.codigo);
        return raiz;
    }
    printf("Nome: ");
    scanf(" %49[^\n]", p.nome);
    printf("Preco (use ponto, ex: 4.50): ");
    scanf("%f", &p.preco);
    printf("Quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    raiz = inserir(raiz, p);
    printf("Produto cadastrado.\n");
    return raiz;
}

void buscarProduto(No* raiz)
{
    int codigo;
    No* achado;

    printf("Codigo: ");
    scanf("%d", &codigo);
    achado = buscar(raiz, codigo);
    if (achado == NULL) {
        printf("Nao existe produto com o codigo %d.\n", codigo);
    } else {
        mostrarProduto(achado->produto);
    }
}

/* operacao adicional. O codigo nao muda porque define a posicao na arvore */
void alterarProduto(No* raiz)
{
    int codigo;
    No* achado;

    printf("Codigo: ");
    scanf("%d", &codigo);
    achado = buscar(raiz, codigo);
    if (achado == NULL) {
        printf("Nao existe produto com o codigo %d.\n", codigo);
        return;
    }
    mostrarProduto(achado->produto);
    printf("Novo preco: ");
    scanf("%f", &achado->produto.preco);
    printf("Nova quantidade: ");
    scanf("%d", &achado->produto.quantidade);
    printf("Produto alterado.\n");
}

int main(void)
{
    No* raiz = NULL;
    int opcao;

    raiz = carregarArquivo(raiz);

    do {
        printf("\n===== ESTOQUE DO MERCADINHO =====\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Buscar produto\n");
        printf("3 - Listar produtos\n");
        printf("4 - Exibir a arvore\n");
        printf("5 - Alterar preco e quantidade\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                raiz = cadastrarProduto(raiz);
                break;
            case 2:
                buscarProduto(raiz);
                break;
            case 3:
                listarEmOrdem(raiz);
                break;
            case 4:
                exibirArvore(raiz, 0, 'R');
                break;
            case 5:
                alterarProduto(raiz);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    salvarArquivo(raiz);
    liberarArvore(raiz);
    return 0;
}
