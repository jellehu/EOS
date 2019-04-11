#include "shell.hh"

int main(){
	std::string input;
	char pc[512];
	int filedes;

	if ((filedes = syscall(SYS_open, "conf.txt", O_RDONLY)) == -1){
		std::cout << "Kan configuratie bestand niet open.\n";
		return 1;
	}

	std::string readprompt="";

	while (syscall(SYS_read, filedes, pc, 1) > 0){
		readprompt+=pc;
	}

	readprompt.pop_back();

	std::string prompt = readprompt;

	while(true){
		std::cout << prompt;
		std::getline(std::cin, input);
		if (input == "new_file") new_file();
		else if (input == "ls") list();
		else if (input == "src") src();
		else if (input == "find") find();
		else if (input == "python") python();
		else if (input == "exit") return 0;
		else if (input == "quit") return 0;
		else if (input == "error") return 1;
		else std::cout << "Commando niet gevonden.\n";

		if (std::cin.eof()){
			return 0;
		}
	}
}

void new_file(){
	std::string str_fname;
	std::string str_tekst;
	std::string quest;

	while(str_fname.empty()){
		std::cout << "Bestand naam: ";
		std::getline(std::cin, str_fname); 
	}

	const char * fname = str_fname.c_str();

	if (syscall(SYS_open, fname, O_RDONLY) > 0){;
		std::cout << "Bestand bestaad al. Wil je doorgaan?";
		std::cout << "(y,N): ";
		std::getline(std::cin, quest);
		std::cout << "\n";
	} else{
		quest="y";
	}

	if (quest=="y"){
		int fd = syscall(SYS_open, fname, O_WRONLY | O_CREAT, mode_t S_IRWXU);
		std::cout << "Schrijf je tekst: \n";
		std::getline(std::cin, str_tekst);
		str_tekst.append("<EOF>");
		const void * tekst = str_tekst.c_str();
		syscall(SYS_write, fd, tekst, str_tekst.size());
	}
}

void list(){
	pid_t pid = fork();
	int status;
	if (pid == 0){
		execlp("ls", "ls", "-la", NULL);
	} else{
		wait(NULL);
	}
}

void find(){
	int pid;
	int pip[2];
	int status;
	std::string str_search;

	std::cout << "Search: ";
	std::getline(std::cin, str_search);
	const char * search = str_search.c_str();

	pipe(pip);
	pid = fork();

	if (pid == 0) {
		dup2(pip[1], 1);
		close(pip[0]);
		close(pip[1]);
		execlp("find", "find", ".", NULL);
	} else{
		wait(NULL);
	}

	pid = fork();

	if (pid == 0){
		dup2(pip[0], 0);
		close(pip[0]);
		close(pip[1]);
		execlp("grep", "grep", search, NULL);
	}

	close(pip[0]);
	close(pip[1]);
	wait(NULL);
}

void python(){
	pid_t pid = fork();
	int status;

	if (pid == 0){
		execlp("python", "python", NULL);
	} else{
		wait(NULL);
	}
}

void src(){
	int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755);
  	char byte[1];
	while(syscall(SYS_read, fd, byte, 1)){
		std::cout << byte;
	}
}
