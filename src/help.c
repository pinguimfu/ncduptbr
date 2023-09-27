/* ncdu - NCurses Disk Usage

  Copyright (c) 2007-2020 Yoran Heling

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "global.h"

#include <ncurses.h>
#include <string.h>


static int page, start;


#define KEYS 19
static const char *keys[KEYS*2] = {
/*|----key----|  |----------------description----------------|*/
        "acima, k", "Mover cursor acima",
      "abaixo, j", "Mover cursor abaixo",
  "-->/enter", "Abrir diretorio",
   "<--/, <, h", "Abrir diretorio anterior",
            "n", "listar por nome (asc/descendente)",
            "s", "listar por tamanho (asc/descendente)",
            "C", "listar por tipo de itens (asc/descendente)",
            "M", "listar por tempo (chave -e)",
            "d", "Deletar item ou diretorio selecionado",
            "t", "Mostrar diretorios primeiro",
            "g", "Mostrar porcentagens/graficos",
            "a", "Mostrar espaco aparente ou espaco em disco",
            "c", "Mostrar contagem de itens-filho",
            "m", "Mostrar ultima data ( chave-e)",
            "e", "Mostrar arquivos ocultos",
            "i", "Informacoes adicionais sobre o item",
            "r", "Recalcular o diretorio atual",
            "b", "Abrir terminal no diretorio atual",
            "q", "Sair do NCDU"
};


#define FLAGS 9
static const char *flags[FLAGS*2] = {
    "!", "Erro de leitura do diretorio",
    ".", "Erro de leitura em sub-diretorio",
    "<", "Arquivo ou diretorio excluido das contagens",
    "e", "Diretorio Vazio",
    ">", "Diretorio em outro sistema de arquivos",
    "@", "Nao e um diretorio (link ou socket)",
    "^", "Pseudo-Sistema Excluido",
    "H", "Mesmo arquivo ja foi contado (hard link)",
    "F", "Link Excluido (FirmLink)",
};

void help_draw() {
  int i, line;

  browse_draw();

  nccreate(15, 60, "ncdu help");
  ncaddstr(13, 42, "Press ");
  uic_set(UIC_KEY);
  addch('q');
  uic_set(UIC_DEFAULT);
  addstr(" to close");

  nctab(30, page == 1, 1, "Teclas");
  nctab(39, page == 2, 2, "Formato");
  nctab(50, page == 3, 3, "Sobre");

  switch(page) {
    case 1:
      line = 1;
      for(i=start*2; i<start*2+20; i+=2) {
        uic_set(UIC_KEY);
        ncaddstr(++line, 13-strlen(keys[i]), keys[i]);
        uic_set(UIC_DEFAULT);
        ncaddstr(line, 15, keys[i+1]);
      }
      if(start != KEYS-10)
        ncaddstr(12, 25, "-- more --");
      break;
    case 2:
      attron(A_BOLD);
      ncaddstr(2, 3, "X  [size] [graph] [file or directory]");
      attroff(A_BOLD);
      ncaddstr(3, 4, "The X is only present in the following cases:");
      line = 4;
      for(i=start*2; i<start*2+14; i+=2) {
        uic_set(UIC_FLAG);
        ncaddstr(++line, 4, flags[i]);
        uic_set(UIC_DEFAULT);
        ncaddstr(line, 7, flags[i+1]);
      }
      if(start != FLAGS-7)
        ncaddstr(12, 25, "-- more --");
      break;
    case 3:
      /* Indeed, too much spare time */
      attron(A_REVERSE);
#define x 12
#define y 3
      /* N */
      ncaddstr(y+0, x+0, "      ");
      ncaddstr(y+1, x+0, "  ");
      ncaddstr(y+2, x+0, "  ");
      ncaddstr(y+3, x+0, "  ");
      ncaddstr(y+4, x+0, "  ");
      ncaddstr(y+1, x+4, "  ");
      ncaddstr(y+2, x+4, "  ");
      ncaddstr(y+3, x+4, "  ");
      ncaddstr(y+4, x+4, "  ");
      /* C */
      ncaddstr(y+0, x+8, "     ");
      ncaddstr(y+1, x+8, "  ");
      ncaddstr(y+2, x+8, "  ");
      ncaddstr(y+3, x+8, "  ");
      ncaddstr(y+4, x+8, "     ");
      /* D */
      ncaddstr(y+0, x+19, "  ");
      ncaddstr(y+1, x+19, "  ");
      ncaddstr(y+2, x+15, "      ");
      ncaddstr(y+3, x+15, "  ");
      ncaddstr(y+3, x+19, "  ");
      ncaddstr(y+4, x+15, "      ");
      /* U */
      ncaddstr(y+0, x+23, "  ");
      ncaddstr(y+1, x+23, "  ");
      ncaddstr(y+2, x+23, "  ");
      ncaddstr(y+3, x+23, "  ");
      ncaddstr(y+0, x+27, "  ");
      ncaddstr(y+1, x+27, "  ");
      ncaddstr(y+2, x+27, "  ");
      ncaddstr(y+3, x+27, "  ");
      ncaddstr(y+4, x+23, "      ");
      attroff(A_REVERSE);
      ncaddstr(y+0, x+30, "NCurses");
      ncaddstr(y+1, x+30, "Disk");
      ncaddstr(y+2, x+30, "Usage");
      ncprint( y+4, x+30, "%s", PACKAGE_VERSION);
      ncaddstr( 9,  7, "Escrito por Yoran Heling <projects@yorhel.nl>");
      ncaddstr(10, 16, "https://dev.yorhel.nl/ncdu/");
      ncaddstr(11, 16,  "Traduzido por Gabriel H Paiva");
      break;
  }
}


int help_key(int ch) {
  switch(ch) {
    case '1':
    case '2':
    case '3':
      page = ch-'0';
      start = 0;
      break;
    case KEY_RIGHT:
    case KEY_NPAGE:
    case 'l':
      if(++page > 3)
        page = 3;
      start = 0;
      break;
    case KEY_LEFT:
    case KEY_PPAGE:
    case 'h':
      if(--page < 1)
        page = 1;
      start = 0;
      break;
    case KEY_DOWN:
    case ' ':
    case 'j':
      if((page == 1 && start < KEYS-10) || (page == 2 && start < FLAGS-7))
        start++;
      break;
    case KEY_UP:
    case 'k':
      if(start > 0)
        start--;
      break;
    default:
      pstate = ST_BROWSE;
  }
  return 0;
}


void help_init() {
  page = 1;
  start = 0;
  pstate = ST_HELP;
}


