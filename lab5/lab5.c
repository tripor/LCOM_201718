#include <minix/syslib.h>
#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test5.h"
#include "pixmap.h"

static int imprimir_uso(char **argv) {
	printf(
			"Nao foram especificados nenhuns argumentos. Porfavor use a seguintes notacoes:\n");
	printf("service run %s -args \"init <mode> <delay>\"\n", argv[0]);
	printf("service run %s -args \"square <x> <y> <size> <color>\"\n", argv[0]);
	printf("service run %s -args \"line <xi> <yi> <xf> <yf> <color>\"\n",argv[0]);
	printf("service run %s -args \"xpm <string> <xi> <yi>\"\n",argv[0]);
	printf("service run %s -args \"move <string> <xi> <yi> <xf> <yf> <speed> <frame_rate>\"\n",argv[0]);
	printf("service run %s -args \"controller\"\n",argv[0]);
	return 0;
}

static unsigned long transform_long(char *str, int base) //funcao retirada de lab2
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("Nao foram usados digitos \"%s\"\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

int escolher_funcao(int argc, char **argv) {
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		unsigned long mode, delay;
		if (argc > 4 || argc < 3) //foram inseridos mais que 3 argumentos e portanto dá erro
				{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 3 argumentos separados por espaco.\n");
			return 1;
		}
		mode = transform_long(argv[2], 16);
		if (mode == ULONG_MAX)
			return 1;
		delay = transform_long(argv[3], 10);
		if (delay == ULONG_MAX)
			return 1;
		printf("video:*video_test_init(mode=%x delay=%lu)\n", mode, delay);
		if (video_test_init(mode, delay) == NULL) {
			printf(
					"Aconteceu algo de errado na funcao video_test_init, esta devolveu NULL.\n");
			return 1;
		}
		return 0;
	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		unsigned long x, y, size, color;
		if (argc > 6 || argc < 5) //foram inseridos mais que 5 argumentos e portanto dá erro
				{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 5 argumento separados por espaco.\n");
			return 1;
		}
		x = transform_long(argv[2], 10);
		if (x == ULONG_MAX)
			return 1;
		y = transform_long(argv[3], 10);
		if (y == ULONG_MAX)
			return 1;
		size = transform_long(argv[4], 10);
		if (size == ULONG_MAX)
			return 1;
		color = transform_long(argv[5], 10);
		if (color == ULONG_MAX)
			return 1;
		printf("video:video_test_square(x=%lu y=%lu size=%lu color=%lu)\n", x,
				y, size, color);
		return video_test_square(x, y, size, color);
	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		unsigned long xi, yi, xf, yf, color;
		if (argc > 7 || argc < 6) //foram inseridos mais que 6 argumentos e portanto dá erro
				{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 6 argumentos separados por espaco.\n");
			return 1;
		}
		xi = transform_long(argv[2], 10);
		if (xi == ULONG_MAX)
			return 1;
		yi = transform_long(argv[3], 10);
		if (yi == ULONG_MAX)
			return 1;
		xf = transform_long(argv[4], 10);
		if (xf == ULONG_MAX)
			return 1;
		yf = transform_long(argv[5], 10);
		if (yf == ULONG_MAX)
			return 1;
		color = transform_long(argv[6], 10);
		if (color == ULONG_MAX)
			return 1;
		printf("video:video_test_line(xi=%lu yi=%lu xf=%lu yf=%lu color=%lu)\n",
				xi, yi, xf, yf, color);
		return video_test_line(xi, yi, xf, yf, color);
	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		unsigned long xi, yi;
		if (argc > 5 || argc < 4) //foram inseridos mais que 5 argumentos e portanto dá erro
				{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 4 argumento separados por espaco.\n");
			return 1;
		}
		xi = transform_long(argv[3], 10);
		if (xi == ULONG_MAX)
			return 1;
		yi = transform_long(argv[4], 10);
		if (yi == ULONG_MAX)
			return 1;
		printf("video:test_xpm(image=%s xi=%lu yi=%lu)\n", argv[2], xi,yi);
		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0)
			return video_test_xpm(pic1,xi,yi);
		if (strncmp(argv[2], "pic2", strlen("pic2")) == 0)
					return video_test_xpm(pic2,xi,yi);
		if (strncmp(argv[2], "pic3", strlen("pic3")) == 0)
					return video_test_xpm(pic3,xi,yi);
		if (strncmp(argv[2], "cross", strlen("cross")) == 0)
					return video_test_xpm(cross, xi, yi);
		if (strncmp(argv[2], "penguin", strlen("penguin")) == 0)
					return video_test_xpm(penguin,xi,yi);
	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		unsigned long xi, yi, xf, yf, speed, frame_rate;
		if (argc > 9 || argc < 8) //foram inseridos mais que 5 argumentos e portanto dá erro
				{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 8 argumento separados por espaco.\n");
			return 1;
		}
		xi = transform_long(argv[3], 10);
		if (xi == ULONG_MAX)
			return 1;
		yi = transform_long(argv[4], 10);
		if (yi == ULONG_MAX)
			return 1;
		xf = transform_long(argv[5], 10);
		if (xf == ULONG_MAX)
			return 1;
		yf = transform_long(argv[6], 10);
		if (yf == ULONG_MAX)
			return 1;
		speed = transform_long(argv[7], 10);
		if (speed == ULONG_MAX)
			return 1;
		frame_rate = transform_long(argv[8], 10);
		if (frame_rate == ULONG_MAX)
			return 1;
		printf("video:test_move(image=%s xi=%lu yi=%lu)\n", argv[2], xi, yi);
		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0)
			return video_test_move(pic1, xi, yi, xf, yf, speed, frame_rate);
		if (strncmp(argv[2], "pic2", strlen("pic2")) == 0)
			return video_test_move(pic2, xi, yi, xf, yf, speed, frame_rate);
		if (strncmp(argv[2], "pic3", strlen("pic3")) == 0)
			return video_test_move(pic3, xi, yi, xf, yf, speed, frame_rate);
		if (strncmp(argv[2], "cross", strlen("cross")) == 0)
			return video_test_move(cross, xi, yi, xf, yf, speed, frame_rate);
		if (strncmp(argv[2], "penguin", strlen("penguin")) == 0)
			return video_test_move(penguin, xi, yi, xf, yf, speed, frame_rate);
	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		return video_test_controller();
	}else {
		printf("A funcao escolhida \"%s\" nao existe.\n", argv[1]);
		return 1;
	}
	return 0;

}

int main(int argc, char **argv) {

	sef_startup();
	if (argc == 1)
		return imprimir_uso(argv); // a função não tem argumentos
	else
		return escolher_funcao(argc, argv);
}
