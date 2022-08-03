#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <string>
#include <cstdlib>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;
 // //g++ snake.cpp -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec
 // g++ snake.cpp -lallegro -lallegro_image
 //./a.exe

const float FPS = 5; //velocidade do jogo
const int SCREEN_W = 500;
const int SCREEN_H = 500;
const int QUAD_SIZE = 20; //tamanho da cobrinha



char MAPA[26][26] =
{
    "1111111111111111111111111",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "0000000000000000000000000",//
    "0000000000000000000000000",// 
    "0000000000000000000000000",//lacunas laterais para a snake passar
    "0000000000000000000000000",//
    "0000000000000000000000000",//
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1111111111111111111111111",
};

int andou[26][26] = {0};
int passo = 0;
int pontuacao= 0;//score

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa   = NULL;
ALLEGRO_BITMAP *quad = NULL;
ALLEGRO_BITMAP *fruta = NULL; //cria a cereja
ALLEGRO_BITMAP *abelha = NULL; //cria a abelha
ALLEGRO_BITMAP *telafinal   = NULL;
ALLEGRO_FONT *font20 = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_SAMPLE *comida = NULL;
ALLEGRO_SAMPLE *gameover = NULL;
ALLEGRO_SAMPLE *bonus = NULL;
ALLEGRO_BITMAP *intro1 = NULL;
ALLEGRO_BITMAP *intro2 = NULL;

int i = 15, j = 12;   //posicao inicial da Snake na matriz
int q = 20;           //tamanho de cada celula no mapa
int posy = i*q;       //posicao da Snake no display
int posx = j*q;
int score = 0;
string texto;


bool redraw = true;
bool sair = false;
bool cima, baixo, esq, dir;
bool intro = true;


int inicializa() {
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

     // Inicialização do add-on para uso de fontes
    al_init_font_addon();

    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon())
    {
        cout << "Falha ao inicializar add-on allegro_ttf.\n";
        return -1;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    intro1 = al_load_bitmap("intro1.bmp"); 
    if(!intro1) {
      std::cout << "Falha ao carregar a intro 1" << endl;
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }

    intro2 = al_load_bitmap("intro2.bmp");
    if(!intro2) {
      std::cout << "Falha ao carregar a intro 2" << endl;
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }

    mapa = al_load_bitmap("map2.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    // Carregando o arquivo de fonte
     
  font20 = al_load_font("fonte.ttf", 20, 0);
  if (!font20)
  {
    //al_destroy_display();
    fprintf(stderr, "Falha ao carregar fonte.\n");
    return -1;
  }

  // instalando som
 
     if (!al_install_audio())
  {
    fprintf(stderr, "Falha ao inicializar áudio.\n");
    return -1;
  }
 
  if (!al_init_acodec_addon())
  {
    fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
    return -1;
  }
 
  if (!al_reserve_samples(3))
  {
    fprintf(stderr, "Falha ao alocar canais de áudio.\n");
    return -1;
  }
 
    comida = al_load_sample("gameover.wav");

  if (!comida)
  {
    fprintf(stderr, "Falha ao carregar comida.\n");
    return -1;
  }
  
  gameover= al_load_sample("gameover.wav");

  if (!gameover)
  {
    fprintf(stderr, "Falha ao carregar gameover.\n");
    return -1;
  }

  bonus = al_load_sample("bonus.wav");

  if (!bonus)
  {
    fprintf(stderr, "Falha ao carregar bonus.\n");
    return -1;
  }

  musica = al_load_audio_stream("mus.ogg", 4, 1024);
  if (!musica)
  {
    fprintf(stderr, "Falha ao carregar audio.\n");

    return -1;
  }

    telafinal = al_load_bitmap("telafinal.bmp");
    if (!telafinal)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }

    quad = al_create_bitmap(QUAD_SIZE, QUAD_SIZE);		
    if(!quad)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    fruta = al_load_bitmap("cereja.tga"); 
    if(!fruta)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_set_target_bitmap(quad);                    //muda destino dos desenhos para o bitmap quad
    al_clear_to_color(al_map_rgb(0, 200, 0));       //limpa e colore de magenta
    
    al_set_target_bitmap(al_get_backbuffer(display));  //muda de volta o destino dos desenhos para o display
    al_draw_bitmap(fruta,0,0,0);       
    

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_bitmap(quad);
        al_destroy_bitmap(fruta);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(abelha);
        return 0;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int main(int argc, char **argv)
{
    if(!inicializa()) return -1;

    int scoretotal = 0;
    int pontuacao = 0;
    int velocidade = FPS;

    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);

    cima = false;
    baixo = false;
    esq = false;
    dir = false;
   

    srand(time(0));//inicializa a funcao rand
    int ifruta=(rand()%23)+1,jfruta=(rand()%23)+1; //posicao aleatoria da cereja 
    int iabelha=(rand()%23)+1;//posicao aleatoria da abelha
    int jabelha=(rand()%23)+1;  
    int passou[500000]; //vetor para guardar posicoes da cobrinha
    int compara=-2;// qtde de quadrados no corpo da snake
    int corpo=3; //tamanho da cobrinha
    int contabelha=0; //contador para o inseto, que acrescenta um valor para cada vez que o jogo "roda" o codigo
     

    while(!sair){
    
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (intro) {
            al_draw_bitmap (intro1,0,0,0);
            al_rest(0.1);
            al_flip_display();
            al_draw_bitmap (intro2,0,0,0);
            al_rest(0.1);
            al_flip_display();

           if (ev.type == ALLEGRO_EVENT_KEY_DOWN) { 
            if (ev.keyboard.keycode == ALLEGRO_KEY_R || ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
                    intro = false;
                   
                
            }  
           } 
        }

        if (!intro) {
            if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                passo++;
                andou[i][j]= passo;
                
                if(cima && MAPA[i-1][j] != '1')     //se pra cima não for parede, anda pra cima.
                {
                    i--;
                    posy = i*q;
                }

                if(cima && MAPA[i][j] == '1')      //se andar pra cima e encostar na parede você perde.
                {
                    al_destroy_audio_stream (musica);
                        al_play_sample(gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_draw_bitmap(telafinal,0,0,0);
                        al_flip_display();
                        al_rest(3);
                        
                        sair = true; 
                        break;
                }

                if(baixo && MAPA[i+1][j] != '1')        //se pra baixo não for parede, anda pra baixo.
                {
                    i++;
                    posy = i*q;
                }

                if(baixo && MAPA[i][j] == '1')      //se andar pra baixo e encostar na parede você perde.
                {
                    al_destroy_audio_stream (musica);
                        al_play_sample(gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_draw_bitmap(telafinal,0,0,0);
                        al_flip_display();
                        al_rest(3);
                        
                        sair = true; 
                        break;
                }

                if(esq && MAPA[i][j] != '1' && j==0)        //se a cobrinha passar pelo limite do mapa da esquerda(onde não há parede), ela deve retornor pelo limite do mapa oposto, na mesma linha.
                {
                    j=26;
                    posx = j*q;
                }

                if(esq && MAPA[i][j-1] != '1')      //se pra esquerda não for parede, anda pra esquerda.
                {
                    j--;
                    posx = j*q;
                }

                if(esq && MAPA[i][j] == '1')        //se andar pra esquerda e encostar na parede você perde.
                {
                    al_destroy_audio_stream (musica);
                        al_play_sample(gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_draw_bitmap(telafinal,0,0,0);
                        al_flip_display();
                        al_rest(3);
                        
                        sair = true; 
                        break;
                }

                if(dir && MAPA[i][j] != '1' && j+1==26)         //se a cobrinha passar pelo limite do mapa da direita(onde não há parede), ela deve retornor pelo limite do mapa oposto, na mesma linha.
                {
                    j=0;
                    posx = j*q;
                }

                else if(dir && MAPA[i][j] != '1')       //se pra direita não for parede, anda pra direita.
                {
                    j++;
                    posx = j*q;
                }

                if(dir && MAPA[i][j] == '1')            //se andar pra direita e encostar na parede você perde.
                {
                        al_destroy_audio_stream (musica);
                        al_play_sample(gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_draw_bitmap(telafinal,0,0,0);
                        al_flip_display();
                        al_rest(3);
                        
                        sair = true; 
                        break;
                }
            
                
                redraw = true;
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                    break;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(ev.keyboard.keycode) 
                {
                case ALLEGRO_KEY_UP:    //setinha pra cima
                    if(!baixo)
                    {
                        cima = true;          
                        baixo = false;
                        esq = false;
                        dir = false;
                    }
                    else
                    {  
                        cima=false;
                        baixo=true; //nao deixa o corpo voltar
                        esq=false;
                        dir=false;
                    }
                    break;

                case ALLEGRO_KEY_DOWN:  //setinha pra baixo
                    if(!cima)
                    {
                        cima = false;
                        baixo = true;
                        esq = false;
                        dir = false;
                    }

                else
                {  
                        cima=true;
                        baixo=false;
                        esq=false;
                        dir=false;
                    }
                    break;

                case ALLEGRO_KEY_LEFT:  //setinha pra esquerda
                    if(!dir)
                    {
                        cima = false;
                        baixo = false;
                        esq = true;
                        dir = false;
                    }
                    else
                    { 
                        cima=false;
                        baixo=false;
                        esq=false;
                        dir=true;
                    }
                    break;

                case ALLEGRO_KEY_RIGHT: //setinha pra direita
                    if(!esq)
                    {
                        cima = false;
                        baixo = false;
                        esq = false;
                        dir = true;
                    }
                    else
                    {  
                        cima = false;
                        baixo = false;
                        esq = true;
                        dir = false;
                    }
                    break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:  //tecla ESC
                    sair = true;
                    break;
                }
            }

            if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;

                al_clear_to_color(al_map_rgb(0,0,0));

                al_draw_bitmap(mapa,0,0,0);
                int contpassou=-1; //contador para inicializar o vetor, descontando o pixel das margens  
                bool comeu=false; //booleano da snake comer uma cereja incia falso
                for(int i=0;i<26;i++)
                    for(int j=0;j<26;j++)
                        if(andou[i][j]>0 && andou[i][j]>=passo-corpo-1){
                            i=passou[(contpassou*2)+1];//acrescenta 1 para que o vetor se inicie em 0
                            j=passou[(contpassou*2)+1];
                             contpassou++;//aumenta o contador a cada "passo"
                            if(j==jfruta&&i==ifruta){ //cobrinha encontrou a cereja
                                comeu=true;//cobrinha comeu a cereja
                                }
                            al_draw_bitmap(quad,j*q,i*q,0);   //desenha quadrado
                        }
                if(comeu){ //caso uma cereja seja colhida
                    al_play_sample(comida, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);//musica para comida
                    score = score + 10;//aumenta a pontuação
                    corpo++;//aumenta o tamanho da cobra
                    velocidade += 1;//velocidade aumenta a cada cereja colhida
                    bool teste=true;//caso teste para a posicao da fruta
                    int k;
                    while(teste){ //enquanto o teste nao der negativo uma nova cereja nao eh criada  
                        teste=false; //apos a cobra comer uma cereja uma nova posicao eh sorteada
                        ifruta=(rand()%23)+1; //gera posicao aleatoria para fruta
                        jfruta=(rand()%23)+1;
                        
                        for(k=(contpassou*2)+1;k>=1;k=k-2){

                            if(ifruta==passou[k]&&jfruta==passou[k-1]){   //posicao da fruta == posicao da cobrinha
                                teste=true;// se for gera uma nova posicao
                    }}}
                }
                al_draw_bitmap(fruta,jfruta*q,ifruta*q,0); //desenha a frutinha
                if (contabelha==99){//quando o contador atinge o valor de 99 que ocorre quando o jogo da 99 "voltas" 
                    bool teste=true;
                    while (teste){  
                        teste=false;
                        iabelha=(rand()%23)+1; // gera posicao aleatória para a abelha
                        jabelha=(rand()%23)+1;
                        for (int k=(contpassou*2)+1;k>=1;k-=2)  //mesmo caso da cereja
                            if((iabelha==passou[k]&&jabelha==passou[k-1])||(iabelha==ifruta && jabelha==jfruta)) 
                            //caso teste para a mesma posicao da fruta e inseto
                                teste=true;
                                }
                    contabelha++; //contador para o inseto
                }
                else  if (contabelha== 100 ){ //quando o contador atinge o valor de 100 que ocorre quando o jogo da 100 "voltas" uma abelha eh desenhada no mapa 
                    abelha = al_load_bitmap ( "abelha.tga" ); //desenha a abelha 
                    al_draw_bitmap (abelha,jabelha*q,iabelha*q, 0 );//desenha a abelha na malha do jogo 
                    if (j==jabelha&&i==iabelha){ //se a posicao da cabeca da cobra for a mesma da abelha
                        al_play_sample(bonus, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        pontuacao=pontuacao + 50; //cada inseto equivale a 5 frutas
                        corpo+=3 ;//o corpo da cobra aumenta 3 unidades
                        velocidade *= 10;//caso coma uma abelha a velocidade aumenta 10x
                        contabelha=0;//zera o contador das abelhas
                        }
                }
                else{   
                    contabelha++; //contador da abelha acumula um valor com inicio 0
                    } 
                if(compara>contpassou){ //a cobrinha encostou no seu proprio corpo, se a 
                        al_destroy_audio_stream (musica); //inicializacao para musica de game over
                        al_play_sample(gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_draw_bitmap(telafinal,0,0,0);
                        al_flip_display();
                        al_rest(3);
                        sair = true; 
                        break;
                }

                    scoretotal = score + pontuacao; //acumulador do score
                    texto =  "Score  " + std::to_string(scoretotal); //exibicao do placar na malha
                    al_draw_text(font20, al_map_rgb(4,250, 196), 250, 480, ALLEGRO_ALIGN_CENTER ,texto.c_str());

                    
                compara=contpassou;
                al_flip_display();
            
            }
        }

    }
  
    al_destroy_bitmap(quad);
    al_destroy_bitmap(fruta);
    al_destroy_bitmap(mapa);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(abelha);
    al_destroy_audio_stream(musica);
    al_destroy_sample(comida);
    al_destroy_sample(gameover);
    al_destroy_sample(bonus);

    return 0;
}