#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <locale>
#include <cmath>
#include <cstdint>
#include <iostream>

using namespace std;

#pragma pack(push)
#pragma pack(1)
struct cabecalho_arq { //cabe�alho de arquivo
    char assinatura[2];
    int32_t tam_arquivo;
    short int reserve_1; // como o int � 4 bytes usamos short
    short int reserve_2; // que ira reduzir para 2 bytes
    int dataOffset;
};

struct cabecalho_bitMapa{ //cabe�alho de mapa de bits
    int tam_cabecalho;
    unsigned int largura_img; // 4 bytes com valores int +
    unsigned int altura_img;
    short int planes; // apenas 2 bytes
    short int bitsPorPixel;
    int compressao;
    int tam_img;
    int PixelsPorMetro_X;
    int PixelsPorMetro_Y;
    int total_cores;
    int cores_imp; // cores importantes

};
#pragma pack(pop)


int main()
{
    setlocale(LC_ALL,"");

    ifstream inFile; //para acessar o arquivo para leitura
    ofstream grayscale_bmp; //para aceesaor o arquivo de relatorio
    ofstream arqmono; // arquivo de img em monocor
    ofstream arqsaida; // arquivo que contem o vetor para ser usado no display nokia
    ofstream corte_img; // arquivo de teste

    string arquivo;// nome do arquivo
    string sufixo;// sufixo bmp

    int sufixo_pos; //posição do sufixo do arquivo

    /**
    -> Entrada do usuario do nome do arquivo
    */
    bool erro = true;
    while(erro)  // enquanto o usuario não colocar o nome
    {
        sufixo_pos = 0;

        cout << "-> DIGITE O NOME DO ARQUIVO PARA LEITURA COM O SEU TIPO:"
             << endl << "Ex: 'nome do arquivo.tipo'" << endl;

        cin >> arquivo;// usuario atribui o nome

        for(int tam = arquivo.length()-1; tam > 0; --tam)  // lê o nome do arquivo inversamente para achar o ponto do tipo de arquivo
        {
            sufixo.push_back(arquivo[tam]); // adiciona o caractere desejado
            sufixo_pos++;
            if(arquivo[tam] == '.')  // se acharmos o "." do sufixo de tipo de arquivo
            {
                erro = false;
                break; //fim do loop
            }
            else if(tam == 1)  // se não houver ".tipo"
            {
                cout << "---> TIPO DE ARQUIVO NAO ESPECIFICADO! TENTE NOVAMENTE <---" << endl << endl;
                sufixo.clear(); // limpando a string para fazer o loop novamente
                break; //fim do loop
            }
        }
    }


    // apaga o sufixo do nome do arquivo dado pelo usuario
    arquivo.erase((arquivo.length() - sufixo_pos), arquivo.length());
    // inverte sufixo pois foi atribuida de trás para frente
    reverse(sufixo.begin(), sufixo.end());

    // Converter a string para maiusculas
    transform(sufixo.begin(), sufixo.end(), sufixo.begin(), ::toupper);

    /**
    -> testando sse o arquivo existe ou é bmp
    */
    if(sufixo != ".BMP") {
        cout << "arquivo informado não é do tipo bmp" << endl;
        return -1;
    }
    
    inFile.open("teste.bmp", ios::in|ios::binary);
    if (!inFile) // testando se o arquivo existe
    {
        cout << "Arquivo saida nao pode ser aberto" << endl;
        return -1;
    }

    cabecalho_arq cab_arq; // cabecalho de arquivo
    cabecalho_bitMapa cab_bit; // cabecalho de mapa de bits

    /**
    -> Informacoes sobre o cabecalho do arquivo e sobre a imagem
    */
    // preenchendo os campos do cabecalho de arquivo
    inFile.read((char *)&cab_arq, sizeof(cabecalho_arq));
    cout << "\tCABECALHO DO ARQUIVO: " << arquivo + sufixo <<  endl;
    cout << "- Assinatura: "<< cab_arq.assinatura[0] << cab_arq.assinatura[1] << endl;
    cout << "- Tamanho do arquivo: " << cab_arq.tam_arquivo << " bytes" << endl;
    cout << "- Offset dos dados: " << cab_arq.dataOffset << " bytes" << endl;

    // preenchendo os campos do cabecalho de mapa de bit
    inFile.read((char *)&cab_bit, sizeof(cabecalho_bitMapa));
    cout << "\n\tCABECALHO DO MAPA DE BITS" << endl;
    cout << "- Planos: " << cab_bit.planes << endl;
    cout << "- Largura: " << cab_bit.largura_img << endl;
    cout << "- Altura: " << cab_bit.altura_img << endl;
    cout << "- Cores usadas: " << cab_bit.total_cores << endl;
    cout << "- Cores importantes: " << cab_bit.cores_imp << endl;
    cout << "- Btes po pixel: " << cab_bit.bitsPorPixel << endl;



    /**
    -> Calculo de numero de bytes e gravacao dos mesmos
    */
    // calculando o resto de bytes em cada linha
    int bytes_por_pixel = cab_bit.bitsPorPixel/8; // quantos bytes tem em cada pixel
    int resto = 0;
    if(cab_bit.largura_img%4 != 0)
    {
        resto = (4 - ((cab_bit.largura_img*bytes_por_pixel)%4));
    }

    int bytes_Linha = (cab_bit.largura_img*bytes_por_pixel) + resto; // numero de bytes em uma linha
    int numBytes = (cab_bit.altura_img * bytes_Linha); // numero de bytes total usados


    int bytes_Linha = (cab_bit.largura_img*bytes_por_pixel) + resto; // numero de bytes em uma linha
    int numBytes = (cab_bit.altura_img * bytes_Linha); // numero de bytes total usados

    uint8_t *rgb = new uint8_t[numBytes]; // todos os bytes de todos os pixels
    uint8_t *mono = new uint8_t[numBytes]; 

    // gravando todos os bytes
    inFile.read((char *)rgb, numBytes);
    inFile.close();


    /**
    -> Insercao de dados para o recorte da imagem
    */
    int x, y;
    int limiar_val;

    // entrada de informações do usuario
    cout << "\n\tDIGITE AS COORDENADAS DO RECORTE - (canto inferior esquerdo)" << endl ;
    int controle = 0;
    while (controle != 2){
        controle = 0;
        cout << "-> Posicao eixo X " << "(0 a " << cab_bit.largura_img - 84 << "): ";
        cin >> x;
        cout << "-> Posicao eixo y " << "(49 a " << cab_bit.altura_img << "): ";
        cin >> y;
        cout << "-> Valor de limiar RGB (0 a 255): ";
        cin >> limiar_val;
        //teste - dentro dos parametros - 49 é o minimo de linhas pq vai de 1 a 48
        if(x + 84 <= cab_bit.largura_img && y - 49 >= 0 && y <= cab_bit.altura_img && x >= 0)
            controle++;
        if(limiar_val <= 255 && limiar_val >= 0)
            controle++;
        if(controle != 2)
        {
            cerr << "\nERRO nos parametros" << endl;
        }
    }

    /**
    -> Criacao dos arquivos em mono e grayscale
    */
    // escrevendo o arquivo de saida rgb para monocor
    arqmono.open("mono.bmp", ios::out);
    arqmono.write((char *)&cab_arq, sizeof(cabecalho_arq));
    arqmono.write((char *)&cab_bit, sizeof(cabecalho_bitMapa));

    // escrevendo o arquivo de saida rgb para grayscale
    grayscale_bmp.open(arquivo + "_gs" + sufixo, ios::out);
    grayscale_bmp.write((char *)&cab_arq, sizeof(cabecalho_arq));
    grayscale_bmp.write((char *)&cab_bit, sizeof(cabecalho_bitMapa));

    for (int y = 0; y < cab_bit.altura_img; y++) {
        for (int x = 0; x < cab_bit.largura_img; x++) {
            // calculando a posição do ponteiro rgb
            int offset = y * bytes_Linha + x * bytes_por_pixel;

            // calculando para grayscale
            int media = (30 * rgb[offset] + 59* rgb[offset+1] + 11* rgb[offset+2])/100;

            // passando os valores de R, G e B para grayscale
            rgb[offset] = media;         // Red
            rgb[offset + 1] = media;     // Green
            rgb[offset + 2] = media;     // Blue

            arqsaida << media<< endl;

            if(media < limiar_val){
                mono[offset] = 0;
                mono[offset + 1] = 0;
                mono[offset + 2] = 0;
            }else{
                mono[offset] = 255;
                mono[offset + 1] = 255;
                mono[offset + 2] = 255;
            }

        }
    }

    arqmono.write((char *)mono, numBytes);
    grayscale_bmp.write((char *)rgb, numBytes);
    grayscale_bmp.close();
    arqmono.close();

    //criando um arquivo para o recorte da imagem
    corte_img.open("arquivoteste.h", ios::out);

     /**
    -> Criando o recorte da imagem
    */

    int valores[48][84];
    y = cab_bit.altura_img - y + 48; // invertedo a coordenada pela imagem estar ivertida
    for(int i = y; i > y -48; i--) // como o processo é retrogrado começamos com o canto inferior esquerdo da matriz e vamos subindo
    {
        for (int j = 0; j < 84; j++)
        {

            int cord_x = x + j;

            int byte_pos = i * bytes_Linha + cord_x * bytes_por_pixel;
            int pixel = mono[byte_pos] + mono[byte_pos + 1] + mono[byte_pos + 2];

            if(pixel == 0)  //pixel ligado na escala de mono
            {
                valores[y - i][j] = 1;
            }
            else
            {
                valores[y - i][j] = 0;
            }

            corte_img << valores[y - i][j]; // imprimindo a imagem de teste

        }
        corte_img << endl;
    }
    corte_img.close();


    /**
    -> Lendo e gravando os valores para a exibição no display nokia
    */
    arqsaida.open(arquivo + ".h", ios::out);
    arqsaida << "uint8_t " + arquivo +"[504] = {" << endl;

    //ponteiro para manipular a imagem recortada
    int *ponteiro = &valores[0][0];
    for(int j = 0; j < 6; j++)
    {
        for(int x1 = 0; x1 < 84; x1++)
        {
            int conta = 0;
            for(int y1 = 0; y1 < 8; y1++)
            {
                if(*(ponteiro + j * (84*8) + (y1* 84) + x1) == 1)  //ponto ligado
                {
                    conta += pow(2, y1);// calculando o valor em hexa
                }
                else
                {
                    conta += 0;
                }

            }

            if(j == 5 && x1 == 83)  // caso chegue no ultimo valor fechar as chaves da variavel
            {
                arqsaida << "0d" << conta << " };";
            }
            else arqsaida << "0d" << conta << ", ";

        }
        arqsaida << endl;
    }
    arqsaida.close();

    cout << "\nAs informacoes foram gravadas no projeto!" << endl;
    delete[] rgb;
    delete[] mono;
    
    return 0;
    }
