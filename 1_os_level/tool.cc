#include <iostream>
#include <string>

std::string translate(int input, std::string variabele){
	std::string result = "";
	int temp;
	input = input - ((input / 26) * 26);
	for (unsigned int i; i < variabele.size(); i++){
		temp = variabele[i];
		if((temp >= 65 && temp <= 90) || (temp >=97 && temp <= 122)){
			temp = variabele[i] + input;
			if((temp < 65 || temp > 90) && (temp < 97 || temp > 122)){
				temp -= 26;
			}
		}
		result += temp;
	}

  	return result;
}

int main(int argc, char *argv[]){
	std::string s;
	int x = atoi(argv[1]);
	char *end;
	int correctInput = strtol(argv[1], &end, 10);

	if(argc != 2){
		std::cerr << "Deze functie heeft exact 1 argument nodig";
    		return -1;
	}

	if (*end != '\0'){
		std::cout << "Deze functie heeft een getal als argument nodig \n";
		return -1;
	}


	while(true){
		std::cin >> s;
    		if(std::cin.eof()){
			return 0;
		}

    		std::cout << translate(x, s) << std::endl;
	}
}
