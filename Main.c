#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
    // Code goes here
    char path[1024];
    strncpy(path, argv[0], sizeof(path));

    // Шукаємо останній слеш
    char *last_slash = strrchr(path, '/');

    if (last_slash != NULL) {
        // Ставимо "нуль-термінатор" на місце слеша.
        // Це фактично обрізає рядок у цій точці.
        *last_slash = '\0'; 
    } else {
        // Якщо слеша немає (програма запущена просто як ./main), 
        // значить ми в поточній папці
        strcpy(path, ".");
    }

    printf("Hello world! path is: %s \n", path); // argv[0] скоріш за все має шлях до файлу (його старт) але не повний а саме той який вводиться консоль 
									 // для запуску (наприклад якщо в консолі уже було прописано
									 // cd /project/folder/ й запущено як ./main то воно отримає тільки ./main
    int skipper = 0;
    if (skipper == 0) {  // якщо аргумент такий, то таке, якщо інший, то інше.
	
	FILE *pipe_in = popen("grim -g \"$(slurp)\" -", "r");

	char final_cmd[2048];
	sprintf(final_cmd, 
        	"appimage-run %s/Tesseract/tesseract-5.5.2-x86_64.AppImage - stdout --tessdata-dir %s/Tesseract | grep -v 'installed in' | wl-copy", 
        path, path);  // Трансформує строку, та записує її в final_cmd
	FILE *pipe_out = popen(final_cmd, "w");
	
	if (!pipe_in || !pipe_out) {
		perror("Помилка відкриття пайпів");
		return 1;
	}

	unsigned char buffer[4096];
	size_t n;

	// Читаємо бінарні дані з першої команди і пишемо в другу
	while ((n = fread(buffer, 1, sizeof(buffer), pipe_in)) > 0) {
	fwrite(buffer, 1, n, pipe_out);
	}

	pclose(pipe_in);
	pclose(pipe_out);

	printf("Зображення передано!\n");

	};
    return 0;
}

