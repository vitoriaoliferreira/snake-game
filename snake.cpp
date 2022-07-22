#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <cstdlib>
using namespace std;
 
 // g++ arquivo.cpp -lallegro -lallegro_image
 //./a.exe

const float FPS = 6;
const int SCREEN_W = 500;
const int SCREEN_H = 500;
const int QUAD_SIZE = 20;
const int FRUTA_SIZE = 10;


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
    "0000000000000000000000000",
    "0000000000000000000000000",
    "0000000000000000000000000",
    "0000000000000000000000000",
    "0000000000000000000000000",
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
int pontuacao= 0;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa   = NULL;
ALLEGRO_BITMAP *quad = NULL;
ALLEGRO_BITMAP *fruta = NULL;

int i = 15, j = 12;   //posicao inicial da Snake na matriz
int q = 20;           //tamanho de cada celula no mapa
int posy = i*q;       //posicao da Snake no display
int posx = j*q;



bool redraw = true;
bool sair = false;
bool cima, baixo, esq, dir;


int inicializa() {
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
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

    mapa = al_load_bitmap("map2.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    quad = al_create_bitmap(QUAD_SIZE, QUAD_SIZE);	
    fruta = al_create_bitmap(FRUTA_SIZE, FRUTA_SIZE);	
    if(!quad)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    if(!fruta)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_set_target_bitmap(quad);                    //muda destino dos desenhos para o bitmap quad
    al_clear_to_color(al_map_rgb(0, 200, 0));       //limpa e colore de magenta
                                                 //muda de volta o destino dos desenhos para o display
    al_set_target_bitmap(fruta);
    al_clear_to_color(al_map_rgb(200, 0, 0));       //limpa e colore de magenta
    al_set_target_bitmap(al_get_backbuffer(display));

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_bitmap(quad);
        al_destroy_bitmap(fruta);
        al_destroy_display(display);
        al_destroy_timer(timer);
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

    cima = false;
    baixo = false;
    esq = false;
    dir = false;

    srand(time(0));
    int ifruta=(rand()%23)+1,jfruta=(rand()%23)+1; //posicao aleatoria da fruta

    int iparede= i;
    int jparede= j; 
    int vet[500000]; //vetor para guardar posicoes da cobrinha
    int compara=-2;//i e j da parede coso a cobrinha encoste
    int corpo=3; //tamanho cobrinha
     

    while(!sair){
    
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            passo++;
            andou[i][j]= passo;
            if(cima && MAPA[i-1][j] != '1')
            {
                i--;
                posy = i*q;
            }

            if(cima && MAPA[i][j] == '1')
            {
                sair = true;
                break;
            }

            if(baixo && MAPA[i+1][j] != '1')
            {
                i++;
                posy = i*q;
            }

            if(baixo && MAPA[i][j] == '1')
            {
                sair = true;
                break;
            }

            if(esq && MAPA[i][j-1] != '1')
            {
                j--;
                posx = j*q;
            }

            if(esq && MAPA[i][j] == '1')
            {
                sair = true;
                break;
            }

            if(dir && MAPA[i][j+1] != '1')
            {
                j++;
                posx = j*q;
            }

            if(dir && MAPA[i][j] == '1')
            {
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
            int cont=-1;  
            int comeu=0; 
            for(int i=0;i<26;i++)
                for(int j=0;j<26;j++)
                    if(andou[i][j]>0 && andou[i][j]>=passo-corpo-1){
                        cont++;
                        vet[cont*2]=j;
                        vet[cont*2+1]=i;
                        if(j==jfruta&&i==ifruta) //cobrinha comeu a fruta
                            comeu=1;
                        al_draw_bitmap(quad,j*q,i*q,0);   //desenha quadrado
                    }
            if(comeu){
                pontuacao++;
                corpo++;
                int teste=1;
                int k;
                while(teste){  
                    teste=0;
                    ifruta=(rand()%23)+1; //gera posicao aleatoria para fruta
                    jfruta=(rand()%23)+1;
                    
                    for(k=cont*3;k>=1;k=k-2){

                        if(ifruta==vet[k]&&jfruta==vet[k-1]){   //posicao da fruta == posicao da cobrinha
                            teste=1;
                }}}
            }
            al_draw_bitmap(fruta,jfruta*q,ifruta*q,0); //desenha fruta
            if(compara>cont){ //a cobrinha encostou no seu proprio corpo
                sair=true;
            }
            compara=cont;
            al_flip_display();
          
        }

    }
  
    al_destroy_bitmap(quad);
    al_destroy_bitmap(fruta);
    al_destroy_bitmap(mapa);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
