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
struct cabecalho_arq   // cabecalho de arquivo
{
    char assinatura[2];
    int32_t tam_arquivo;
    int16_t reserve_1; // como o int 4 bytes usamos short
    int16_t reserve_2; // que ira reduzir para 2 bytes
    int32_t dataOffset;
};

struct cabecalho_bitMapa  // cabecalho de mapa de bits
{
    int32_t tam_cabecalho;
    uint32_t largura_img; // 4 bytes com valores int +
    uint32_t altura_img;
    int16_t planes;       // apenas 2 bytes
    int16_t bitsPorPixel;
    int32_t compressao;
    int32_t tam_img;
    int32_t PixelsPorMetro_X;
    int32_t PixelsPorMetro_Y;
    int32_t total_cores;
    int32_t cores_imp; // cores importantes

};
#pragma pack(pop)


int main()
{
    setlocale(LC_ALL,"");

    ifstream inFile; // para acessar o arquivo para leitura
    ofstream grayscale_bmp; // para aceesaor o arquivo de relatorio
    ofstream arqmono; // arquivo de img em monocor
    ofstream arqsaida;
    ofstream corte_img; // arquivo de teste


    /**
    -> Entrada do usuario do nome do arquivo
    */

    string arquivo; // nome do arquivo
    string sufixo;  // sufixo bmp
    int sufixo_pos; // posição do sufixo do arquivo

    bool erro = true;
    while(erro)  // enquanto o usuario nao colocar o nome
    {
        sufixo_pos = 0;

        cout << "Insira o nome do arquivo com a extensao (ex.: teste.bmp): ";
        cin >> arquivo;

        for(int tam = arquivo.length()-1; tam > 0; --tam)  // lê o nome do arquivo inversamente para achar
        {                                                  // o ponto do tipo de arquivo
            sufixo.push_back(arquivo[tam]); // adiciona o caractere desejado
            sufixo_pos++;
            if(arquivo[tam] == '.')  // se acharmos o "." do sufixo de tipo de arquivo
            {
                erro = false;
                break;
            }
            else if(tam == 1)  // se não houver ".tipo"
            {
                cout << "ERRO: arquivo nao reconhecido. Tente novamente." << endl << endl;
                sufixo.clear(); // limpando a string para fazer o loop novamente
                break;
            }
        }
    }


    // apaga o sufixo do nome do arquivo dado pelo usuario
    arquivo.erase((arquivo.length() - sufixo_pos), arquivo.length());

    // inverte sufixo pois foi atribuida de trás para frente
    reverse(sufixo.begin(), sufixo.end());

    // converter a string para maiusculas
    transform(sufixo.begin(), sufixo.end(), sufixo.begin(), ::toupper);

    cout << "Arquivo inserido: " << arquivo+sufixo << endl << endl;



    /**
    -> Testando se o arquivo existe e se eh .BMP
    */

    if(sufixo != ".BMP") {
        cout << "ERRO: o arquivo informado nao eh do tipo BMP." << endl;
        return -1;
    }
    inFile.open(arquivo + sufixo, ios::in|ios::binary);
    if (!inFile) // testando se o arquivo existe
    {
        cout << "ERRO: o arquivo saida nao pode ser aberto." << endl;
        return -1;
    }



    /**
    -> Informacoes sobre o cabecalho do arquivo e sobre a imagem
    **/

    cabecalho_arq cab_arq; // cabecalho de arquivo
    cabecalho_bitMapa cab_bit; // cabecalho de mapa de bits

    // preenchendo os campos do cabecalho de arquivo
    inFile.read((char *)&cab_arq, sizeof(cabecalho_arq));
    cout << "\tCABECALHO DO ARQUIVO: " << endl;
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
    cout << "- Bytes por pixel: " << cab_bit.bitsPorPixel << endl;



    /**
    -> Calculo de numero de bytes e gravacao dos mesmos
    */

    int bytes_por_pixel = cab_bit.bitsPorPixel/8; // quantos bytes tem em cada pixel
    int resto = 0;
    if(cab_bit.largura_img%4 != 0) // calculando o resto de bytes em cada linha
    {
        resto = (4 - ((cab_bit.largura_img*bytes_por_pixel)%4));
    }

    int bytes_Linha = (cab_bit.largura_img*bytes_por_pixel) + resto; // numero de bytes em uma linha
    int numBytes = (cab_bit.altura_img * bytes_Linha); // numero de bytes total usados

    uint8_t *rgb = new uint8_t[numBytes]; // todos os bytes de todos os pixels
    uint8_t *mono = new uint8_t[numBytes];

    inFile.read((char *)rgb, numBytes); // gravando todos os bytes da imagem
    inFile.close();



    /**
    -> Insercao de dados para o recorte da imagem pelo usuario
    */

    int x, y;
    int limiar_val;

    cout << "\n\tDIGITE AS COORDENADAS DO RECORTE" << endl ;
    cout << "     (a partir do canto inferior esquerdo)" << endl;
    int controle = 0;
    while (controle != 2)
    {
        controle = 0;
        cout << "-> Posicao eixo X " << "(0 a " << cab_bit.largura_img - 84 << "): ";
        cin >> x;
        cout << "-> Posicao eixo y " << "(49 a " << cab_bit.altura_img << "): ";
        cin >> y;
        cout << "-> Valor de limiar RGB (0 a 255): ";
        cin >> limiar_val;

        // teste dos parametros, 49 é o minimo de linhas porque vai de 1 a 48
        if(x + 84 <= cab_bit.largura_img && y - 49 >= 0 && y <= cab_bit.altura_img && x >= 0)
            controle++;
        if(limiar_val <= 255 && limiar_val >= 0)
            controle++;
        if(controle != 2)
        {
            cerr << "\nERRO: limite dos parametros. Insira conforme as informacoes." << endl;
        }
    }



    /**
    -> Criacao do arquivo de grayscale
    */

    grayscale_bmp.open(arquivo + "_gs" + ".bmp", ios::out);
    grayscale_bmp.write((char *)&cab_arq, sizeof(cabecalho_arq));
    grayscale_bmp.write((char *)&cab_bit, sizeof(cabecalho_bitMapa));

    /**
    -> Criacao do arquivo de mono
    */

    arqmono.open(arquivo + "_mono" + sufixo, ios::out);
    arqmono.write((char *)&cab_arq, sizeof(cabecalho_arq));
    arqmono.write((char *)&cab_bit, sizeof(cabecalho_bitMapa));


    for (int y = 0; y < cab_bit.altura_img; y++)
    {
        for (int x = 0; x < cab_bit.largura_img; x++)
        {
            // calculando a posicao do ponteiro rgb
            int offset = y * bytes_Linha + x * bytes_por_pixel;

            // calculando para grayscale
            int media = (30 * rgb[offset] + 59* rgb[offset+1] + 11* rgb[offset+2])/100;

            // passando os valores de R, G e B para grayscale
            rgb[offset] = media;     // Red
            rgb[offset + 1] = media; // Green
            rgb[offset + 2] = media; // Blue

            // criando a imagem em mono
            if(media < limiar_val)
            {
                mono[offset] = 0;
                mono[offset + 1] = 0;
                mono[offset + 2] = 0;
            }
            else
            {
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



    /**
    -> Criando o recorte da imagem
    */

    int valores[48][84];
    y = cab_bit.altura_img - y + 48; // invertendo a coordenada pela imagem estar invertida
    for(int i = y; i > y -48; i--) // comecando pelo o canto inferior esquerdo da matriz e
    {                              // vamos subindo
        for (int j = 0; j < 84; j++)
        {

            int cord_x = x + j;

            int byte_pos = i * bytes_Linha + cord_x *bytes_por_pixel;
            int pixel = mono[byte_pos] + mono[byte_pos + 1] + mono[byte_pos + 2];

            if(pixel == 0)  // pixel ligado na escala de mono
            {
                valores[y - i][j] = 1;
            }
            else
            {
                valores[y - i][j] = 0;
            }

        }

    }

    // criando um arquivo para o recorte da imagem
    corte_img.open("arquivoteste.h", ios::out);

    // imprimindo a imagem recortada
    for(int p = 0; p < 48; p++){
        for(int o = 0; o < 84; o++){
            corte_img << valores[p][o];
        }
        corte_img << endl;
    }
    corte_img.close();



    /**
    -> Lendo e gravando os valores para a exibição no display nokia
    */

    arqsaida.open(arquivo + ".h", ios::out);
    arqsaida << "uint8_t " + arquivo +"[504] = {" << endl;

    int *ponteiro = &valores[0][0];
    for(int j = 0; j < 6; j++)
    {
        for(int x1 = 0; x1 < 84; x1++)
        {
            int conta = 0;
            for(int y1 = 0; y1 < 8; y1++)
            {
                if(*(ponteiro + (j * 672) + (y1* 84) + x1) == 1)  // ponto ligado
                {
                    conta += pow(2, y1); // calculando o valor em hexadecimal
                }
                else conta += 0;

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