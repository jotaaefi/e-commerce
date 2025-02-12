#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

const int TAM_MAX = 100;

typedef struct Produto
{
    int codigo;
    char descricao[40];
    char categoria;
    int qtdEstoque;
    int qtdNOCarrinho;
    float precoUnitario;
    int desconto;
    float valorCompra;
    float valorJ;
   
};

struct Carrinho
{
    Produto Produtos_Carrinho[TAM_MAX];
    int qtdCarrinho[TAM_MAX];
    int contadorItensCarrinho = 0;
	float totalCarrinho = 0;
    int qtdPedidos = 0;

};

struct Data {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
    int segundo;
};

struct Pedido
{
    Data dt;
    Produto produtos[TAM_MAX];
    int numero;
    int contProdutos = 0;
    float total = 0;
    long long cpf;
};



void menu_principal(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho, Pedido pedidos[], int &qtdPedidos);
void menu_carrinho(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho,Pedido pedidos[], int &qtdPedidos);
void menu_produtos(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho,Pedido pedidos[], int &qtdPedidos);
void incluirProduto(Produto produto[], int &qtd);
void excluirProduto(Produto produto[], int &qtd);
void incluirNoCarrinho(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho);
void excluirProdutoCarrinho(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho);
void aumentarQuantidade(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho);
void diminuirQuantidade(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho);
void esvaziarCarrinho(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho);
void consultar_categoria(Produto produto[], int &qtd);
void leValidadeC();

void data_hora_atual(int &dia, int &mes, int &ano,int &hora, int &min, int &seg)
{
    time_t t = time(NULL);
    struct tm lt = *localtime(&t);
    ano = lt.tm_year + 1900;
    mes = lt.tm_mon + 1;
    dia = lt.tm_mday;
    hora = lt.tm_hour;
    min = lt.tm_min;
    seg = lt.tm_sec;
}

Data data_atual() {
    Data dt;
    data_hora_atual(dt.dia, dt.mes, dt.ano, dt.hora, dt.minuto, dt.segundo);

    return dt;
}

void consultar_preco(Produto produto[], int &qtd)
{
    if(qtd == 0){
        puts("Nao ha produtos cadastrados");
    }
    else{
        bool trocou = true;
   
        for(int k= qtd-1; k > 0 && trocou; k--)
        {
        trocou = false;
            for(int i = 0; i < k; i++)
            {
                if(produto[i].precoUnitario > produto[i+1].precoUnitario)
                {
                    Produto e = produto[i];
                    produto[i] = produto[i+1];
                    produto[i+1] = e;
                    trocou = true;
                }
            }
        }
        puts("------------------------------------------------------------------------------");
        puts("Codigo Descricao                               Categ.  Qtd    Preco  Desconto");
        puts("------------------------------------------------------------------------------");
        for(int i = 0; i < qtd; i++){
       printf("%03d    %-40s  %c     %03d %7.2f     %02d\n", produto[i].codigo, produto[i].descricao, produto[i].categoria, produto[i].qtdEstoque, produto[i].precoUnitario, produto[i].desconto);
        }
        puts("------------------------------------------------------------------------------");
    }    
}

void consultar_categoria(Produto produto[], int &qtd)
{
    if(qtd == 0){
        puts("Nao ha produtos cadastrados");
    }
    else{
        bool trocou = true;
       
        for (int k = qtd-1; k > 0 && trocou; k--)
        {
       
            trocou = false;
            for (int i = 0; i < k; i++)
            {
                int num = tolower(produto[i].categoria);
                int num2 = tolower(produto[i+1].categoria);
                if (num > num2)
                        {
                            Produto aux = produto[i];
                            produto[i] = produto[i+1];
                            produto[i+1] = aux;
                            trocou = true;
                        }
                else if(num == num2){
					if(strcmpi(produto[i].descricao, produto[i+1].descricao) > 0){
							Produto aux = produto[i];
                            produto[i] = produto[i+1];
                            produto[i+1] = aux;
                            trocou = true;
					}
				}        
            }
        }
       
   
        puts("------------------------------------------------------------------------------");
        puts("Codigo Descricao                               Categ.  Qtd    Preco  Desconto");
        puts("------------------------------------------------------------------------------");
        for(int i = 0; i < qtd; i++){
        printf("%03d    %-40s  %c     %03d %7.2f     %02d\n", produto[i].codigo, produto[i].descricao, produto[i].categoria, produto[i].qtdEstoque, produto[i].precoUnitario, produto[i].desconto);
        }
        puts("------------------------------------------------------------------------------");
    }    
}




int Le_Codigo()
{
    int codigo;
    do{
        printf("Codigo: ");
          scanf("%d",&codigo);
            if(codigo < 1 || codigo > 999)
        printf("\nDigite um valor de codigo valido: ");
    }while(codigo  < 1 || codigo > 999);

   
    return codigo;
}

bool cpfValido(long long cpf)
{
    int primeiro_dv, segundo_dv, soma, resto, j, k;
    long long numero_cpf;
    int mult[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    if (cpf < 11111111111L || cpf > 99999999999L || cpf % 11111111111L == 0)
        return false;


    primeiro_dv = cpf % 100 / 10,
    segundo_dv  = cpf % 10;

    numero_cpf = cpf / 100; 
    soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (numero_cpf % 10) * mult[i];
        numero_cpf /= 10;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1)
        j = 0;
    else
        j = 11 - resto;


    if (j != primeiro_dv)
        return false;

    numero_cpf = cpf / 10;  // NÃºmero do cpf sem segundo DV
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (numero_cpf % 10) * mult[i];
        numero_cpf /= 10;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1)
    
        k = 0;
    else
        k = 11 - resto;

    
    if (k != segundo_dv)
        return false;

    return true;
}

long long le_cpf() 
{
    long long cpf;
    bool valido = false;

        do
        {
            printf("\nCPF: ");
            scanf("%lld", &cpf);
            getchar();
            cpfValido(cpf);
            
            if (cpfValido(cpf) == false)
            {
                printf("CPF INVALIDO, DIGITE NOVAMENTE: ");
            }
            else
            {
                printf("CPF VALIDO!\n");
                return cpf;
            }
        
        }while(cpfValido(cpf) == false);
        
    }   

void convform(char lerCc[], int LeCc2[])
{

        int a = 0;
        for (int i=0; i < 20; i++)
         {
            if (lerCc[i] == '.' || lerCc[i] == '-')
                {
                    i = i+1;
                }
            LeCc2[a] = lerCc[i]-48;
            a = a+1;
         }
}

//multiplica posicao impar , soma aos demais numeros e valida
bool multi(int LeCc2[])
{
     
     int acum = 0;
     int total = 0;

     for(int i = 0; i < 15; i++)
      {
        acum = LeCc2[i] * 2;
        if (acum >9)
            {
            acum=acum-9;
            }
        total = total+acum;
        i = i+1;
      }
        for (int i = 1; i < 16;i++)
        {
            total=total+LeCc2[i];
            i=i+1;
        }
        int modulo;
        modulo = total % 10;
        if (modulo == 0)
        {
            if (total<150)
                {
                    printf("Cartao: ");
                    for (int i = 0; i < 16;i++)
                        {
                            if (i == 4 || i == 8 || i == 12 )
                            {
                            printf(" ");
                            }
                            printf("%d",LeCc2[i]);
                        }
                        printf("\n");
                    return true;
                }
        }
        else
            {
            printf("Cartao Invalido\n\n");
            return false;
            }

}

bool tipoCartao(int Lecc2[])
{
    int tipo = Lecc2[0];
   
    switch(tipo)
    {
    case  4:
   
        printf("Tipo: VISA\n");
        return true;
        break;
    case 5:
        printf("Tipo: MasterCard\n");
        return true;
        break;
   
    default:
        printf("Operadora invalida\n");
        return false;
        break;
    }


}

void leCvv(int &cvv )
{
    do
    {
        printf("CVV: ");
        scanf("%d",&cvv);
        if (cvv < 111 || cvv > 999)
        {
            printf("Digite novamente: ");
        }
       
    } while (cvv < 111 || cvv > 999);


}

void leCc(char lerCartao[], int LerCartaoLimpo[])
{
    bool valido = false;
    int cvv;

       do
       {
            printf("Digite seu cartao (1234-1234-1234-1234 ou 1234123412341234)\nCartao: >> ");
            fflush(NULL);
            gets(lerCartao);
            convform(lerCartao, LerCartaoLimpo);
            valido = multi(LerCartaoLimpo);
            if (valido == true)
            {
                valido = tipoCartao(LerCartaoLimpo);
            }
       }while(valido == false);
       
        leCvv(cvv);
        leValidadeC();
       
    return;
}


void leValidadeC()
{
    Data dataAtual;
    dataAtual = data_atual();
    bool valido = false;

    int mes, ano;
    do
    {
    printf("Data de validade\n");
        do
        {
            printf("Mes: ");
            scanf("%d",&mes);
            if (mes < 01 || mes > 12)
            {
               printf("Erro, digite um mes valido!\n");
            }
           
        } while (mes < 01 || mes > 12);
       
        do
        {
           printf("Ano: ");
            scanf("%d", &ano);
            if (ano > 2050)
            {
                printf("Erro, digite digite um ano valido!\n");
            }
           
        } while(ano > 2050);

    if (ano < dataAtual.ano)
    {
        printf("Erro, data invalida\n");
        valido = false;
    }
    else if(ano == dataAtual.ano && mes < dataAtual.mes)
    {
        printf("Erro, data invalida\n");
        valido = false;
    }
    else
    {

        valido = true;
    }
    }while(valido == false);
   
}

void Le_string(char nome[], char tag[]) {
    char str_temp[1000];

    while(true) {
        printf("%s", tag);
        gets(str_temp);
        if (strlen(str_temp) < 4 || strlen(str_temp) > 40)
            puts("\nErro, digite novamente: \n");
        else break;
    }

    strcpy(nome, str_temp);
}


void LerCategoria(char &categoria)
{
    //char categoria;
    do
       {
            printf("Categoria: ");
            categoria = getchar();
           
            
            if(categoria != 'A' && categoria != 'a' && categoria != 'B' && categoria != 'b' && categoria != 'C' && categoria != 'c' && categoria != 'D'&& categoria != 'd' && categoria != 'E' && categoria != 'e')
            {
                printf("Erro, digite uma categoria valida\n");
                getchar();
            }
            towupper(categoria);  // erro so tirar 
       }while(categoria != 'A' && categoria != 'a' && categoria != 'B' && categoria != 'b' && categoria != 'C' && categoria != 'c' && categoria != 'D'&& categoria != 'd' && categoria != 'E' && categoria != 'e');
       getchar();


}

int Le_QtdEstoque()
{
   int qtdEstoque;
     
    do
        {
            printf("Quantidade em estoque: ");
                scanf("%d",&qtdEstoque);
                if(qtdEstoque < 1 || qtdEstoque > 9999)
                    printf("Erro, digite um valor valido\n ");
        } while (qtdEstoque < 1 || qtdEstoque > 9999);

 return qtdEstoque;  

}


void trim(char str[]) {
    int i, j;

    // Limpa os brancos do fim
    j = strlen(str)-1;
    while (j >= 0 && str[j] == ' ')
        str[j--] = 0;

    // Procura o 1o caracter diferente de branco ou o fim da string
    j = 0;
    while (str[j] != 0 && str[j] == ' ')
       j++;

     // Se a string nÃ£o estÃ¡ vazia, remove os brancos do inicio
    if (str[j] != 0) {
        i = 0;
        while (str[j] != 0)
            str[i++] = str[j++];
        str[i] = 0;
    }
}

int conta_decimais(const char str[])
{
    int i = 0, dec = 0;

    // Se comeÃ§a com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Pula os digitos atÃ© o ponto
    while (str[i] != 0 && str[i] != '.')
        i++;

    // Conta os digitos apÃ³s o ponto
    if (str[i] == '.') {
        i++;
        while (str[i] != 0) {
            i++;
            dec++;
        }
    }

    return dec;
}

bool eh_numero_real(const char str[]) {
    bool achou_ponto = false;
    int i = 0;

    // Se comeÃ§a com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Verifica se a string sÃ³ tem dÃ­gitos e, no mÃ¡ximo, um ponto
    while (str[i] != 0) {
        if (! isdigit(str[i]))
            if (str[i] == '.') {
                // Verifica se Ã© ou nÃ£o o primeiro ponto que encontrou
                if (achou_ponto)
                    return false;
                else
                    achou_ponto = true;
            }
            else
                return false;
        i++;
    }

    return true;
}

void le_string(char buffer[], int max) {
    // LÃª no mÃ¡ximo "max-1" caracteres
    fgets(buffer, max, stdin);

    // Substitui o ultimo caracter por NULO se ele for igual a \n
    int tam = strlen(buffer);

    if (buffer[tam-1] == '\n')
        buffer[tam-1] = 0;

    // Limpar o que nÃ£o foi lido pelo fgets
    //fflush(stdin);
}

void le_string(const char label[], const char msg_erro[], char str[], int min, int max) {
    char buffer[1000]; // Buffer temporÃ¡rio para leitura dos dados
    int t;

    do {
        printf("%s", label);
        le_string(buffer, 1000);
        trim(buffer);
        t = strlen(buffer);
        if (t < min || t > max)
            puts(msg_erro);
    } while (t < min || t > max);

    // Copia o dado lido para str
    strcpy(str, buffer);
}

double le_real(const char label[], const char msg_erro[], int min_dec, int max_dec, double min, double max) {
    bool valido;
    char buffer[31]; // Assume que um nÃºmero nÃ£o vai ter mais de 30 dÃ­gitos
    double n;

    do {
        // Assume que um nÃºmero nÃ£o vai ter mais de 30 dÃ­gitos
        le_string(label, msg_erro, buffer, 0, 30);

        // Verifica a string Ã© um numero real
        if (! eh_numero_real(buffer))
            valido = false;
        else {
            // Conta as casas decimais para verificar se estÃ¡ no intervalo definido
            int decimais = conta_decimais(buffer);

            if (decimais < min_dec || decimais > max_dec)
                valido = false;
            else {
                // Converte a string em double
                n = strtod(buffer, NULL);

                // Verifica se N estÃ¡ no intervalo definido
                valido = n >= min && n <= max;
            }
        }

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    return n;
}


float Le_PrecoU()
{
    float p;
     do{    
	 		getchar();   
            p =  le_real("Preco: ", "Preco deve ser maior que zero com ate duas casas decimais", 0, 2, 0.01, 9999999.99);
                
        } while(p < 0.01 || p > 9999999.99);
        return p;
}

int ler_Desconto()
{
    int desconto;
        do{
              printf("Desconto: ");
                 scanf("%d",&desconto);
            if(desconto < 0 || desconto > 99)
               {
                    printf("Erro, Digite um valor valido:\n ");
               }
        } while (desconto < 0 || desconto > 99);
    return desconto;
}
   

int localizar_codigo(Produto p[], int i, int codigo) {

    int inicio = 0;
    int fim = i-1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        if (p[meio].codigo == codigo)
            return meio;
        else if (p[meio].codigo  > codigo)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }

    return -1;
}


void organizar_codigo(Produto p[], int i) {
    bool trocou = true;

    for (int k = i-1; k > 0 && trocou; k--) {
        trocou = false;
        for (int j = 0; j < k; j++) {
            if ( p[j].codigo> p[j+1].codigo) {
                Produto aux = p[j+1];
                p[j+1] = p[j];
                p[j] = aux;
                trocou = true;
            }
        }
    }
}


void incluirProduto(Produto produto[], int &qtd)
{
         
         Produto p_temp; //temporia para armazenar o produto

        if (qtd >= TAM_MAX)
            {
            puts("\nErro: Quantidade maxima de produtos!\nExclua um produto para poder adicionar outro.");
                return;
            }

        while(true)
         {
            p_temp.codigo = Le_Codigo();
            if(localizar_codigo(produto, qtd+1, p_temp.codigo) != -1 && qtd != 0)
                puts("\nErro: Codigo ja cadastrado!");
            else break;
         }
         getchar();

            Le_string(p_temp.descricao, "Descricao: ");
            //getchar();
           
            LerCategoria(p_temp.categoria);
            //getchar();
       
            p_temp.qtdEstoque = Le_QtdEstoque(); // FUNCIONANDO
            p_temp.precoUnitario = Le_PrecoU(); // FUNCIONANDO
            p_temp.desconto = ler_Desconto(); // FUNCIONANDO

            puts("\nProduto Cadastrado com sucesso!");
           
            //strupr(p_temp.descricao);
            produto[qtd] = p_temp;

        //organizar_codigo(produto, qtd);
        qtd++;
   
   return;
}    
   

void excluirProduto(Produto produto[], int &qtd)
{
    //organizar_codigo(produto, qtd);
    int codigo;

    codigo = Le_Codigo();

   int index = localizar_codigo(produto, qtd, codigo);

    if (index == -1)
        puts("\nErro: Codigo nao cadastrado!");
    else
    {
    	if(produto[index].qtdEstoque > 0){
    		printf("Erro: estoque maior que zero\n");
		}
		else{
        	for (int j = index; j < qtd - 1; j++)
        	{
            	produto[j] = produto[j+1];
        	}
        	puts("\nExclusao de produto realizada!");
        	qtd--;
    	}
    }
    //organizar_codigo(produto, qtd);
    return;
}

void alteracaoProduto(Produto produto[], int &qtd)
{
  //organizar_codigo(produto, qtd);
    Produto p_temp;
    int codigo;
    codigo = Le_Codigo();
		
    int index = localizar_codigo(produto, qtd+1, codigo);
    if (index == -1)
        	printf("\nErro: Codigo nao cadastrado!\n");
   else{
    	
    	int auxQtd;
       auxQtd = p_temp.qtdEstoque;
       do
        {
                printf("Quantidade em estoque: ");
                scanf("%d",&p_temp.qtdEstoque);
                if(p_temp.qtdEstoque < -1 || p_temp.qtdEstoque > 9999)
                    printf("Erro, digite um valor valido\n ");
                else if(p_temp.qtdEstoque < produto[index].qtdEstoque && p_temp.qtdEstoque != 0) 
					printf("Erro, quantidade em estoque nao pode ser diminuida\n");   
        } while (p_temp.qtdEstoque < -1 || p_temp.qtdEstoque > 9999 || p_temp.qtdEstoque < produto[index].qtdEstoque && p_temp.qtdEstoque != 0);
       
       if (p_temp.qtdEstoque == 0)
       {
         p_temp.qtdEstoque = auxQtd;
         printf("O valor nao foi alterado!\n");  
       }
       
       else
       {
        produto[index].qtdEstoque = p_temp.qtdEstoque;  
        printf("\nQuantidade alterada com sucesso!\n");
       }		
    	
       int auxPreco;
        auxPreco = p_temp.precoUnitario;
       
        do{
       		   
	 		getchar();   
            p_temp.precoUnitario =  le_real("Preco a ser alterado: ", "Preco deve ser maior que zero com ate duas casas decimais", 0, 2, 0.00, 9999999.99);
           
                if(p_temp.precoUnitario < -1.00 || p_temp.precoUnitario > 9999999.99)
                    printf("Erro, digite um valor valido\n ");
        }while(p_temp.precoUnitario < -1.00 || p_temp.precoUnitario > 9999999.99);
        if (p_temp.precoUnitario == 0)
        {
            p_temp.precoUnitario = auxPreco;
            printf("\nO preco nao foi alterado!\n");  
        }

       else
       {
            produto[index].precoUnitario = p_temp.precoUnitario;
            printf("\nO preco foi alterado!\n");
       }
       
	
	int auxDesconto;
        auxDesconto = p_temp.desconto;
        do{
       

                printf("Digite o desconto para ser alterado: ");
                 scanf("%d",&p_temp.desconto);
            if(p_temp.desconto < -2 || p_temp.desconto > 99)
               {
                    printf("Erro, Digite um valor valido:\n ");
               }
        } while (p_temp.desconto < -2 || p_temp.desconto > 99);
        if (p_temp.desconto == -1)
        {
           p_temp.desconto = auxDesconto;
           printf("O desconto nao foi alterado!\n");
           
        }
        else
        {
            produto[index].desconto = p_temp.desconto;
            printf("Desconto alterado com sucesso!\n");
        }   
       
    //Desconto: valor inteiro de 0 a 99. Se o usuÃ¡rio digitar -1 o desconto atual nÃ£o deve ser alterado.
   	}
        
    //organizar_codigo(produto, qtd);
    return;
}

/*Consulta de produtos por categoria
a) Os produtos devem ser apresentados de acordo com o layout do protÃ³tipo e devem estar ordenados por categoria e, dentro da categoria, por descriÃ§Ã£o (sempre em ordem crescente).
b) Se nÃ£o houver produtos cadastrados deve ser apresentada a mensagem "NÃ£o hÃ¡ produtos cadastrados".
Consulta de produtos por preÃ§o
c) Os produtos devem ser apresentados de acordo com o layout do protÃ³tipo e devem estar ordenados por preÃ§o (em ordem crescente).
d) Se nÃ£o houver produtos cadastrados deve ser apresentada a mensagem "NÃ£o hÃ¡ produtos cadastrados".
*/




void incluirNoCarrinho(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho)
{
//Produto p_temp;
   
    char resp;
    int codigo;
    bool achou = false;
    bool continua = true;
    int posicaoProduto;

//Dentro do loop do carrinho
    do
    {
         codigo = Le_Codigo();
         for (int i = 0; i < qtd; i++)
        {
            if(produto[i].codigo == codigo)//Se achou o produto, o posicao recebe a posicao dele DUH
            {
               if (produto[i].qtdEstoque <= 1)
               {
                printf("Produto com estoque insuficiente!\n");
                continua = false;
               }
               
                posicaoProduto = i;
                achou = true;
               for(int k = 0; k < carrin[qtdCarrinho].contadorItensCarrinho; k++)
               {
                if(carrin[qtdCarrinho].Produtos_Carrinho[k].codigo == codigo) // Se achar o codigo que ja foi incluido, ele vai pedir outro codigo
                {
                    achou = false; // Vai cair no while, ou ja foi incluido ou nao existe
                }

               }
               
            }
       
        }
        if(achou == false)
            {
               
                printf("Nao existe produto com esse codigo ou produto ja foi incluido!");
                 getchar();
                 printf("\nDeseja continuar(S / N)?");
                 resp = getchar();
                 
                 if (resp == 'N'|| resp == 'n')
                    {
                        continua = false;
                        achou = true;
                    }
                 //Vai ficar preso no loop, ate achar um codigo existente

            }
       
    }while(achou == false);
   //Saiu do loop
   if (continua == true)
   {
    int quantidade;
        do{
            printf("Quantidade solicitada: ");
            scanf("%d", &quantidade);
            if(quantidade < 0 || quantidade > produto[posicaoProduto].qtdEstoque){
                printf("Erro, digite novamente!\n");
}
        }while(quantidade < 0 || quantidade > produto[posicaoProduto].qtdEstoque);
    //Ã© necessÃ¡rio informar o cÃ³digo do produto e a quantidade.
        float desconto = 1 - ((float) produto[posicaoProduto].desconto  / 100.00);
         
        produto[posicaoProduto].qtdNOCarrinho = quantidade;
        produto[posicaoProduto].qtdEstoque -= quantidade;
        printf("Produto incluido no carrinho!\n");
        carrin[qtdCarrinho].Produtos_Carrinho[carrin[qtdCarrinho].contadorItensCarrinho] = produto[posicaoProduto];
        carrin[qtdCarrinho].qtdCarrinho[carrin[qtdCarrinho].contadorItensCarrinho] = quantidade;
        produto[posicaoProduto].valorCompra = carrin[qtdCarrinho].Produtos_Carrinho[posicaoProduto].precoUnitario * desconto * carrin[qtdCarrinho].Produtos_Carrinho[posicaoProduto].qtdNOCarrinho;  
         carrin[qtdCarrinho].totalCarrinho +=  produto[posicaoProduto].valorCompra;
       
         /*(float)carrin[qtdCarrinho].Produtos_Carrinho[carrin[qtdCarrinho].contadorItensCarrinho].qtdEstoque 
        * (carrin[qtdCarrinho].Produtos_Carrinho[carrin[qtdCarrinho].contadorItensCarrinho].precoUnitario 
        - ((float)carrin[qtdCarrinho].Produtos_Carrinho[carrin[qtdCarrinho].contadorItensCarrinho].
         * carrin[qtdCarrinho].Produtos_Carrinho[carrin[qtdCarrinho].contadorItensCarrinho].precoUnitario) 
         * ((float)carrin[qtdCarrinho].Produtos_Carrinho[carrin[qtdCarrinho].contadorItensCarrinho].desconto / 100.00 ));
         * */
        carrin[qtdCarrinho].contadorItensCarrinho++;  
   }
 
}

void excluirProdutoCarrinho(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho)
{
    int codigo;
    bool achou = false;
    char resp;
    do
    {
         codigo = Le_Codigo();
         for (int l = 0; l < carrin[qtdCarrinho].contadorItensCarrinho; l++)
        {
            if(carrin[qtdCarrinho].Produtos_Carrinho[l].codigo == codigo)
            {
                achou = true;
               
                carrin[qtdCarrinho].Produtos_Carrinho[l].codigo = 0;
                printf("Produto excluido do carrinho!\n");
                for (int i = 0; i < qtd; i++)
                    {
                        if(produto[i].codigo == codigo){                              
                           produto[i].qtdEstoque += carrin[qtdCarrinho].qtdCarrinho[l]; // aumentando com a antiga quantidade que estava no carrinho
                        }
           
                    }
               
            }
       
        }
        if(achou == false){
               
            printf("Nao existe nenhum produto com esse codigo no carrinho!");
            getchar();
            printf("\nDeseja continuar(S / N)?");
            resp = getchar();
                 
                 if (resp == 'N'|| resp == 'n')
                    {
                        achou = true;
                    }
                 //Vai ficar preso no loop, ate achar um codigo existente
            }
       
    }while(achou == false);
}

void aumentarQuantidade(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho)
{
    int codigo;
    bool achou = false;
    bool continua = true;
    char resp;
    do
    {
         codigo = Le_Codigo();
         for (int l = 0; l < carrin[qtdCarrinho].contadorItensCarrinho; l++)
        {
            if(carrin[qtdCarrinho].Produtos_Carrinho[l].codigo == codigo)
                {
                    for (int i = 0; i < qtd; i++)
                    {
                        if(produto[i].codigo == codigo && produto[i].qtdEstoque > 1)//Se a qtd do estoque for maior que 1
                        {                              
                           produto[i].qtdEstoque--;
                           carrin[qtdCarrinho].qtdCarrinho[l]++;
                           printf("Quantidade do produto aumentada!\n");
                           achou = true;
                                         
                        }
           
                    }
                }
           
        }

        if (achou == false)
        {
           printf("Produto nao existe no carrinho ou quantidade insuficiente no estoque!\n");
           getchar();
                 printf("\nDeseja continuar(S / N)?");
                 resp = getchar();
                 
                 if (resp == 'N'|| resp == 'n')
                    {
                        achou = true;
                    }
        }
       
    }while(achou == false);
}


void diminuirQuantidade(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho)
{
    int codigo;
    bool achou = false;
    bool continua = true;
    char resp;
    do
    // i = produto
    // l = carrinho
    {
         codigo = Le_Codigo();
         for (int l = 0; l < carrin[qtdCarrinho].contadorItensCarrinho; l++)
        {
            if(carrin[qtdCarrinho].Produtos_Carrinho[l].codigo == codigo)
                {
                    for (int i = 0; i < qtd; i++)
                    {
                        if(produto[i].codigo == codigo)
                        {                              
                           produto[i].qtdEstoque++;
                           carrin[qtdCarrinho].qtdCarrinho[l]--;
                           if (carrin[qtdCarrinho].qtdCarrinho[l] == 0)
                            {
                                carrin[qtdCarrinho].Produtos_Carrinho[l].codigo = 0;
                            }
                           printf("Quantidade do produto diminuida!\n");
                           achou = true;
                                         
                        }
                       
           
                    }
                }
           
        }

        if (achou == false)
        {
           printf("Produto nao existe no carrinho!\n");
           getchar();
                 printf("\nDeseja continuar(S / N)?");
                 resp = getchar();
                 
                 if (resp == 'N'|| resp == 'n')
                    {
                        achou = true;
                    }
        }
       
    }while(achou == false);
}



void esvaziarCarrinho(Produto produto[], int &qtd, Carrinho carrin[], int &qtdCarrinho)
{
    bool vazio = true;

    for (int l = 0; l < carrin[qtdCarrinho].contadorItensCarrinho; l++)
    {
      if (carrin[qtdCarrinho].Produtos_Carrinho[l].codigo != 0)
      {
        for (int i = 0; i < qtd; i++)
                    {
                        if(produto[i].codigo == carrin[qtdCarrinho].Produtos_Carrinho[l].codigo)
                        {                              
                          produto[i].qtdEstoque += carrin[qtdCarrinho].qtdCarrinho[l];
                           
                            vazio = false;            
                        }
                    }
       
      }
     
    }
   
    if (vazio == true)
    {
        printf("O carrinho ja esta vazio!\n");
    }
    else
    {
        qtdCarrinho++; // Passar para o proximo carrinho
        printf("O carrinho foi esvaziado!\n");
    }
   
}

bool CarrinhoVazio(Carrinho carrin[], int &qtdCarrinho)
{
    if (carrin[qtdCarrinho].contadorItensCarrinho == 0)
    {
        printf("\nCarrinho vazio!\n");
        return true;
    }
    for (int i = 0; i < carrin[qtdCarrinho].contadorItensCarrinho; i++)
    {
       if (carrin[qtdCarrinho].Produtos_Carrinho[i].codigo != 0)
       {
            return false;
       }
       
    }
   
   
   
}



void concluirCompra(Carrinho carrin[], int &qtdCarrinho, Pedido pedido[], int &qtdPedidos)
{
    Data dt_atual;
    char lerCartao[20];
    int LerCartaoLimpo[20];
    long long cpf;  
    if (CarrinhoVazio(carrin,qtdCarrinho))
    {
        return;
    }
    
    getchar();
    cpf = le_cpf();
    getchar();

    leCc(lerCartao, LerCartaoLimpo);
    getchar();

    dt_atual = data_atual();

    pedido[qtdPedidos].numero = qtdPedidos; // cadastrar o primeiro pedido e contando
    pedido[qtdPedidos].cpf = cpf; // cadastrar o cpf
    pedido[qtdPedidos].dt = dt_atual; // pegar a data

    for(int i = 0; i < carrin[qtdCarrinho].contadorItensCarrinho; i++)
    {
        if (carrin[qtdCarrinho].Produtos_Carrinho[i].codigo != 0)
        {

           
            pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos] = carrin[qtdCarrinho].Produtos_Carrinho[i];
            pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos].qtdEstoque = carrin[qtdCarrinho].qtdCarrinho[i];
            pedido[qtdPedidos].total += (float)pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos].qtdEstoque * pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos].precoUnitario - ((float)pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos].qtdEstoque * pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos].precoUnitario) * ((float)pedido[qtdPedidos].produtos[pedido[qtdPedidos].contProdutos].desconto / 100.00); //PASSAR O DESCONTO
            pedido[qtdPedidos].contProdutos++;
        }
     
       
    }
   

    qtdPedidos++;// passar para o proximo pedido
    qtdCarrinho++;  // e para o proximo carrinho
       
}

/*Consultar pedidos
a) A listagem dos pedidos deve estar ordenada de forma decrescente por data/hora do pedido.
b) Dentro de cada pedido, os produtos devem estar ordenados de forma crescente por descriÃ§Ã£o.
*/

void ordenaPedidoProduto(Pedido pedido[], int &qtdPedidos) {
    bool trocou = true;
   
    for(int j = 0; j < qtdPedidos; j++)
    {

   
        for (int k = pedido[j].contProdutos - 1; k > 0 && trocou; k--) // K vai percorrer todos os produtos do pedido [j]
        {
            trocou = false;
            for (int i = 0; i < k; i++)
                if (strcmp(pedido[j].produtos[i].descricao, pedido[j].produtos[i+1].descricao )>0)
                {
                    Pedido aux;
                    aux.produtos[0] = pedido[j].produtos[i+1];
                    pedido[j].produtos[i+1] = pedido[j].produtos[i];
                    pedido[j].produtos[i] = aux.produtos[0];
                    trocou = true;
                }
        }
    }
}

bool dataMaisAtual(Pedido pedido1, Pedido pedido2)
{
    if (pedido1.dt.ano > pedido2.dt.ano)
    {
        return true;
    }
    else if(pedido1.dt.ano == pedido2.dt.ano && pedido1.dt.mes > pedido2.dt.mes)
    {
        return true;
    }
    else if(pedido1.dt.ano == pedido2.dt.ano && pedido1.dt.mes == pedido2.dt.mes && pedido1.dt.dia > pedido2.dt.dia)
    {
        return true;
    }
    else if(pedido1.dt.ano == pedido2.dt.ano && pedido1.dt.mes == pedido2.dt.mes && pedido1.dt.dia == pedido2.dt.dia && pedido1.dt.hora > pedido2.dt.hora)
    {
        return true;
    }
     else if(pedido1.dt.ano == pedido2.dt.ano && pedido1.dt.mes == pedido2.dt.mes && pedido1.dt.dia == pedido2.dt.dia && pedido1.dt.hora == pedido2.dt.hora && pedido1.dt.minuto > pedido2.dt.minuto)
    {
        return true;
    }
    else if(pedido1.dt.ano == pedido2.dt.ano && pedido1.dt.mes == pedido2.dt.mes && pedido1.dt.dia == pedido2.dt.dia && pedido1.dt.hora == pedido2.dt.hora && pedido1.dt.minuto == pedido2.dt.minuto && pedido1.dt.segundo > pedido2.dt.segundo)
    {
        return true;
    }
    else
    {
        return false;
    }
   
   
}


void ordenaPedido(Pedido pedido[], int &qtdPedidos)
{
  bool trocou = true;
 
  for (int k = qtdPedidos - 1; k > 0 && trocou; k--) {
      trocou = false;
      for (int i = 0; i < k; i++)
   // Compara v[i] com seu sucessor
   if (dataMaisAtual(pedido[i],pedido[i+1])) {
   // Troca se v[i] Ã© maior que seu sucessor
       Pedido aux = pedido[i + 1];
       pedido[i + 1] = pedido[i];
       pedido[i] = aux;
       trocou = true;
   }
  }
}


void consultarPedidos(Pedido pedido[], int &qtdPedidos)
{
    ordenaPedido(pedido,qtdPedidos);
    ordenaPedidoProduto(pedido, qtdPedidos);
    for(int i = 0; i < qtdPedidos - 1 ; i++)
    {
       
        printf("Numero do pedido: %d\n",pedido[i].numero);
        printf("Data: %02d/ %02d/ %04d\n ",pedido[i].dt.dia,pedido[i].dt.mes, pedido[i].dt.ano);
        printf("Horario: %02d: %02d: %02d\n",pedido[i].dt.hora,pedido[i].dt.minuto,pedido[i].dt.segundo);

        puts("------------------------------------------------------------------------------");
        puts("Codigo Descricao                               Categ.  Qtd    Preco  Desconto");
        puts("------------------------------------------------------------------------------");
        for(int j = 0; j < pedido[i].contProdutos; j++)
        {
            printf("%03d    %-40s  %c     %03d %7.2f     %02d\n", pedido[i].produtos[j].codigo, pedido[i].produtos[j].descricao, pedido[i].produtos[j].categoria,pedido[i].produtos[j].qtdEstoque, pedido[i].produtos[j].precoUnitario, pedido[i].produtos[j].desconto);
           
           
        }
            printf("Total: %2f\n",pedido[i].total); //Printar o total
            puts("------------------------------------------------------------------------------\n");
    }
       
}

void ImprimirProdutosCarrinho(Carrinho c[], int &qtdCarrinho, Pedido pedidos[], int &qtdPedidos)
{

     puts("------------------------------------------------------------------------------");
    puts("Codigo Descricao                               Categ.  Qtd    Preco  Desconto  Valor");
    puts("------------------------------------------------------------------------------");
    for(int i = 0; i < c[qtdCarrinho].contadorItensCarrinho; i++)
    {
     	printf("%03d    %-40s  %c     %03d %7.2f     %02d  %7.2f\n", c[qtdCarrinho].Produtos_Carrinho[i].codigo,c[qtdCarrinho].Produtos_Carrinho[i].descricao, c[qtdCarrinho].Produtos_Carrinho[i].categoria, 
        c[qtdCarrinho].Produtos_Carrinho[i].qtdNOCarrinho,
        c[qtdCarrinho].Produtos_Carrinho[i].precoUnitario, 
        c[qtdCarrinho].Produtos_Carrinho[i].desconto, 
        c[qtdCarrinho].Produtos_Carrinho[i].valorCompra);	
		
    }

    puts("------------------------------------------------------------------------------");
	printf("\nTotal:   R$ %7.2f \n",c[qtdCarrinho].totalCarrinho);
    puts("------------------------------------------------------------------------------");

}

void menu_carrinho(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho,Pedido pedidos[], int &qtdPedidos)
{
    if(CarrinhoVazio(c, qtdCarrinho) ==  false){
    	ImprimirProdutosCarrinho(c,qtdCarrinho, pedidos, qtdPedidos);
	}
    
	    
	    int opcao;
	    printf("=================================\n");
	    printf("Menu do Carrinho\n");
	    printf("=================================\n");
	  printf("1-Incluir\n2-Excluir\n3-Aumentar\n4-Diminuir\n5-Esvaziar\n6-Comprar\n7-Voltar\n");
	do{
	printf("Opcao: ");
	  scanf("%d", &opcao);
	  if(opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5 && opcao != 6 && opcao != 7){
	  puts("Opcao invalida");
	}
	}while(opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5 && opcao != 6 && opcao != 7);
	
	switch(opcao){
		case 1:
			incluirNoCarrinho(p, qtd, c, qtdCarrinho);
			break;
		case 2:
			excluirProdutoCarrinho(p, qtd, c, qtdCarrinho);
			break;
		
		case 3:
			aumentarQuantidade(p, qtd, c, qtdCarrinho);
			break;
		
		case 4:
			diminuirQuantidade(p, qtd, c, qtdCarrinho);
			break;
		
		case 5:
			esvaziarCarrinho(p, qtd, c, qtdCarrinho);
			break;
		
		case 6:
			concluirCompra(c, qtdCarrinho, pedidos, qtdPedidos);
			break;
		case 7:
			menu_principal(p, qtd, c, qtdCarrinho, pedidos, qtdPedidos);
			return;

	}
}


void menu_produtos(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho, Pedido pedidos[], int &qtdPedidos)
{
	int opcao;
	printf("=================================\n");
	printf("Menu de Produtos\n");
	printf("=================================\n");
	printf("1-Incluir\n2-Excluir\n3-Alterar\n4-Consultar por Categoria\n5-Consultar por preco\n6-Voltar\n");
	
	do{
		printf("Opcao: ");
	  	scanf("%d", &opcao);
	  	if(opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5 && opcao != 6){
	  		puts("Opcao invalida");
		}
	}while(opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5 && opcao != 6);
	
	switch(opcao){
		case 1:
			incluirProduto(p, qtd);
			break;
		case 2:
			excluirProduto(p, qtd);
			break;
		case 3:
			alteracaoProduto(p, qtd);
			break;
		case 4:
			consultar_categoria(p, qtd);
			break;
		case 5:
			consultar_preco(p, qtd);
			break;
		case 6:
			menu_principal(p, qtd, c, qtdCarrinho, pedidos, qtdPedidos);
			return;
	}
}

void menu_principal(Produto p[], int &qtd, Carrinho c[], int &qtdCarrinho, Pedido pedidos[], int &qtdPedidos)
{
	int opcao;
	printf("=================================\n");
	printf("E-Commerce - Menu Principal\n");
	printf("=================================\n");
	printf("1-Carrinho\n2-Produtos\n3-Pedidos\n4-Fim\n");
	do{
		printf("Opcao: ");
		scanf("%d", &opcao);
		if(opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4){
		  puts("Opcao invalida");
	}
	}while(opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4);

	switch(opcao){
		case 1:
			menu_carrinho(p, qtd, c, qtdCarrinho, pedidos, qtdPedidos);
			break;
		case 2:
			menu_produtos(p, qtd, c, qtdCarrinho,pedidos,qtdPedidos);
			break;
		case 3:
			consultarPedidos(pedidos,qtdPedidos);
			break;
		case 4:
			exit(0);
	}
}

int main()
{
   
    int qtdProdutos = 0;
    int qtdCarrinho = 0;
    int qtdPedidos = 1;
    Pedido pedidos[TAM_MAX];
    Produto p[TAM_MAX];
    Carrinho c[TAM_MAX];

    while(true)
    {
       menu_principal(p, qtdProdutos, c, qtdCarrinho, pedidos, qtdPedidos);
    }
    Produto listaProdutos[TAM_MAX];
   

//NÃ£o esquecer de mudar o tamanho MXIMO NO FINAL(!!!)
}