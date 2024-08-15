#include <iostream>
#include <cstdlib>
#include <string>
#include <locale>
#include <cmath>
#include <vector>
using namespace std;
void rewrite(int&, int&, int&, string*&, int*&, double*&);
void rewrite1(int, string&);
void check_symbol1(int, string&, string, int&);
void check_if_del_or_umn(int, string&, vector <int>&, int&);
void check_if_minus_or_plus(int, string&, int&);
void check_errors(int&, string&, int&, vector <int>&);
string check_errors_player(string);
string check_tochek(string, int&, string&);
void up_numbers(string&, double*&, int&);
void delenie_deistv_ot_chisel(string&, string&, double*&, string*&, int*&, int&, int&);
void schet_viragenia(int&, string&, double*&, string*&, int*&, int&);
void privedenie(string&);
double calculate(string&, int&, int&);


static void rewrite(int& max_index, int& kolvo_deistv, int& i, string*& deistv, int*& deistv_weight, double*& numbers) { // перезапись большого количества данных в процессе работы калькуляторa. Все перезаписывает налево на 1 символ
	for (int j = max_index; j < kolvo_deistv - i; j++) { // следующий символ становится на место настоящего, из-за чего после метода символ с индексом max_index пропадет
		if (j != kolvo_deistv - 1) {  // если действие не последнее, происходит перезапись
			deistv[j] = deistv[j + 1];
			deistv_weight[j] = deistv_weight[j + 1];
			numbers[j + 1] = numbers[j + 2];
		}
		else { // если действие последнее, ничего не меняется, кроме веса - оно заменяется на отрицательное
			deistv_weight[j] = -1;
		}
	}
}
void rewrite1(int h, string& input) { // метод перезаписывает строку, смещая все символы input налево на 1 символ
	for (int j = h; j < input.length(); j++) { // следующий символ становится на место настоящего, из-за чего после метода символ с индексом h пропадет
		input[j] = input[j + 1];
	}
}
void check_symbol1(int i, string& input, string str, int& check) { // метод для проверки следующего символа: если это не цифра, то check изменится и это отразится на ошибках
	if ((input[i] == '.') || (input[i] == ',') || (input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/') || (input[i] == '^')) { // Это просто вывод строки, если символ с индексом i - действие. Меняет check, которые отвечает за работоспособность
		check = 0;
	}
}
void check_if_del_or_umn(int i, string& input, vector <int>& buf_minus, int& check) { //это тот же метод, что и предыдущий, но с исключением на минус. При встрече минуса, в отдельный вектор будет записан номер числа, и его потом конвертируют для правильности счета
	if (input[i + 1] == '-') { // Если второй символ минус, создается буфер, в который будут записываться индексы чисел, которые необходимо будет конвертироватm
		int buf = 0;
		for (int j = 0; j < i + 1; j++) { // идет подсчет действий до двойного, чтобы понять номер действия
			if ((input[j] == '+') || (input[j] == '-') || (input[j] == '*') || (input[j] == '/') || (input[j] == '^')) {
				buf += 1;
			}
		}
		buf_minus.push_back(buf); //в буфере хранится номер числа, которое будет конвертироваться. Оно будет записано в массив 
		buf = 0;
		rewrite1(i + 1, input); //необходимо удалить минус, для чего и перезаписывается число
	}
	else { // Если второй символ не число и не минус, ошибка
		check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд", check);
	}
}
void check_if_minus_or_plus(int i, string& input, int& check) { // все то же самое, что и в прошлом, но без конвертаций
	if (input[i + 1] == '-') { // Если первый символ минус, 2 минуса становятся плюсом. Если первый символ плюс, просто перезапись
		if (input[i] == '-') {
			input[i + 1] = '+'; // согласно логике второе действие становится плюсом, и оно же становится на место первого, оставаясь единственным
		}
		rewrite1(i, input); // перезапись числа со сдвигом налево на 1 символ
	}
	else { //ну и если второй символ не число или минус, ошибка
		check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд", check);
	}
}
void check_errors(int& check, string& input, int& check_minus, vector <int>& buf_minus) { // один из методов поиска ошибок
	if (check == 1) { //если до сих пор ошибки нет, проводится поиск дальше
		for (int i = 0; i < input.length(); i++) { // цикл проверок на ошибки пользователя
			if (input[0] == '-') {// если первый символ действие и он минус, значение check_minus примет 1 и произойдет перезапись строки, удалив первый минус. В дальнейшем первое число будет конвертированно
				check_minus = 1;
				rewrite1(0, input);
				check_symbol1(0, input, "Ошибка.Действие не может быть первым символом", check); // проверка на наличие первым символом не числа. Любое - ошибка
			}
			if (i == (input.length() - 1)) { //проверка последнего символа. Любой - ошибка
				check_symbol1((input.length() - 1 - check_minus), input, "Ошибка.Действие не может быть последним символом", check);
			}
			else { // проверка на двойные действия
				if (input[i] == '+') { //при первом плюсе может быть только минус - писать 7 + +8 бредятина. Если второй минус, из двух символов делается просто минус и перезаписывается
					check_if_minus_or_plus(i, input, check); // Если второй НЕ минус, ошибка
				}
				else if (input[i] == '-') { // при первом минусе может быть только минус - писать 7 - + 8 бредятина. Если второй минус, из двух символов делается просто плюс и перезаписывается. Если второй НЕ минус, ошибка
					check_if_minus_or_plus(i, input, check);// Если второй НЕ минус, ошибка
				}
				else if (input[i] == '*') { // при первом умножении может быть только минус. Если не он - ошибка, если минус, индекс числа записывается в буфер, дальше по коду оно инвертируется
					check_if_del_or_umn(i, input, buf_minus, check); // Если второй НЕ минус, ошибка
				}
				else if (input[i] == '/') { // при первом делении может быть только минус. Если не он - ошибка, если минус, индекс числа записывается в буфер, дальше по коду оно инвертируется
					check_if_del_or_umn(i, input, buf_minus, check); // Если второй НЕ минус, ошибка
				}
				else if (input[i] == '^') {// после  степени может быть толлько минус
					check_if_del_or_umn(i, input, buf_minus, check); //Если второй НЕ минус, ошибка
				}
			}
		}
	}
}
string check_errors_player(string input) { //один из методов поиска ошибок
	int check = 1;
	for (int i = 0; i < input.length(); i++) { // цикл проверок на ошибки пользователя
		if (input[0] != '-') {// если первый символ - действие, и он минус, произойдет перезапись строки, удалив первый минус. В дальнейшем первое число будет конвертированно
			check_symbol1(0, input, "Ошибка.Действие не может быть первым символом \n", check); // проверка на наличие первым символом не числа. Любое - ошибка
			if (check == 0) {
				return "Действие не может быть первым символом";
			}
		}
		else {
			check_symbol1(1, input, "Ошибка.Не может быть 2 действия подряд \n", check);
			if (check == 0) {
				return "Не может быть 2 действия подряд";
			}
		}
		if (i == (input.length() - 1)) { //проверка последнего символа. Любой - ошибка
			check_symbol1((input.length() - 1), input, "Ошибка.Действие не может быть последним символом \n", check);
			if (check == 0) {
				return "Действие не может быть последним символом";
			}
			else {
				return "";
			}
		}
		else { // проверка на двойные действия
			if (input[i] == '+') { //при первом плюсе может быть только минус - писать 7 + +8 бредятина. Если второй минус, из двух символов делается просто минус и перезаписывается
				if (input[i + 1] != '-') {
					check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд \n", check);
				}
				if (check == 0) {
					return "Не может быть 2 действия подряд";
				}
			}
			else if (input[i] == '-') { // при первом минусе может быть только минус - писать 7 - + 8 бредятина. Если второй минус, из двух символов делается просто плюс и перезаписывается. Если второй НЕ минус, ошибка
				if (input[i + 1] != '-') {
					check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд \n", check);
				}
				if (check == 0) {
					return "Не может быть 2 действия подряд";
				}
			}
			else if (input[i] == '*') { // при первом умножении может быть только минус. Если не он - ошибка, если минус, индекс числа записывается в буфер, дальше по коду оно инвертируется
				if (input[i + 1] != '-') {
					check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд \n", check);
				}
				if (check == 0) {
					return "Не может быть 2 действия подряд";
				}
			}
			else if (input[i] == '/') { // при первом делении может быть только минус. Если не он - ошибка, если минус, индекс числа записывается в буфер, дальше по коду оно инвертируется
				if (input[i + 1] != '-') {
					check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд \n", check);
				}
				if (check == 0) {
					return "Не может быть 2 действия подряд";
				}
			}
			else if (input[i] == '^') { // после  степени в принципе не может ничего быть пока что. Для обработки минуса нужно менять код на работу с вещественными числами
				if (input[i + 1] != '-') {
					check_symbol1(i + 1, input, "Ошибка.Не может быть 2 действия подряд \n", check);
				}
				if (check == 0) {
					return "Не может быть 2 действия подряд";
				}
			}
		}
	}
}
string check_tochek(string input, int& check_tochki, string& errors){ //метод ничего не меняет. Только для отсчета ошибок в написании чисел
	string bufer; // буфер для чисел
	for (int i = 0; i < input.length(); i++) { // деление действий от чисел. В буфер зщаписывается число до тех пор, пока не встретится действие. deistv_weight - вес действия, записывается столько же, сколько действий
		if ((input[i] != '+') && (input[i] != '-') && (input[i] != '*') && (input[i] != '/') && (input[i] != '^')) { //если символ НЕ действие, заполняется буфер
			bufer += input[i];
		}
		else { // При встрече действия буфер проверяется
			int schet_toch = 0;
			for (int j = 0; j < bufer.size(); j++) { // тут считают количество точек
				if (bufer[j] == '.' || bufer[j] == ',') {
					schet_toch += 1;
				}
			}
			if (schet_toch > 1) { //если больше 1 точки, то это ошибка
				return "Ошибка.У вещественных чисел только 1 точка";
			}
			bufer = "";
		}
		if (i == input.length() - 1) { // если точка последняя, то это ошибка
			if (input[i] == ',' || input[i] == '.') {
				return "Ошибка.Точка не может быть последним символом";
			}
			else {
				int schet_toch = 0;
				for (int j = 0; j < bufer.size(); j++) { // тут считают количество точек
					if (bufer[j] == '.' || bufer[j] == ',') {
						schet_toch += 1;
					}
				}
				if (schet_toch > 1) { //если больше 1 точки, то это ошибка
					return "Ошибка.У вещественных чисел только 1 точка";
				}
				else {
					return errors;
				}
			}
		}
	}
}
void up_numbers(string& buf, double*& numbers, int& kolvo_numbers) { //метод для записи чисел в массив. Ему дали buf - число в виде строки - он его проверяет и записывает исходят из его пренадлежности (вещестенное или целое)
	int schet_toch = 0;
	for (int j = 0; j < buf.length(); j++) { // цикл считает количество точек и запятых в числе. Должно быть или 0, или 1
		if (buf[j] == '.' || buf[j] == ',') {
			schet_toch += 1;
		}
	}
	if (schet_toch == 1) { // если есть точка/запятая, надо создать вещественное число, отделив целую часть от дробной и сложив их
		string ah = ""; //"целая переменная" для целой части
		string al = ""; //"дробная переменная" для дробной части
		int trig = 0; //при встрече запятой оно будет изменятся
		for (int j = 0; j < buf.length(); j++) {
			if (trig != 1) { // если тригер 0, в целую переменную будет записываться по цифре, пока не встретит запятую
				if (buf[j] != '.' && buf[j] != ',') {
					ah += buf[j];
				}
				else { // тригер изменится, если будет запятая
					trig = 1;
				}
			}
			else { // после запятой тригер будет 1, а в дробную переменную запишется оставшаяся часть числа
				al += buf[j];
			}
		}
		numbers[kolvo_numbers] = stod(ah) + (stod(al) / pow(10, al.length())); // в массив записывается число: ah менять не надо, а вот al делится на 10 в степени длины al, чтобы число превратилось в дробную часть с целым 0
	}
	else if (schet_toch != 1) { // если у числа нет точки, значит оно целое и можно его просто записать в массив
		numbers[kolvo_numbers] = stod(buf);
	}
	schet_toch = 0;
}
void delenie_deistv_ot_chisel(string& input, string& buf, double*& numbers, string*& deistv, int*& deistv_weight, int& kolvo_numbers, int& check_tochki){ // метод для нахождения чисел в запросе. Он записывает в buf цифры, пока не встретит действие. Дальше buf передастся в предыдущий метод
	for (int i = 0; i < input.length(); i++) { // деление действий от чисел. В буфер зщаписывается число до тех пор, пока не встретится действие. deistv_weight - вес действия, записывается столько же, сколько действий
		if ((input[i] != '+') && (input[i] != '-') && (input[i] != '*') && (input[i] != '/') && (input[i] != '^')) { //если символ НЕ действие, заполняется буфер
			buf += input[i];
		}
		else { // При встрече действия в массив чисел записывается переведенный и проверенный буфер (методом up_numbers), а дальше в массивы заполняются действие и его вес
			up_numbers(buf, numbers, kolvo_numbers);
			if (check_tochki != 0) {
				deistv[kolvo_numbers] = input[i];
				// Запись в массив веса символа. Степень>умножение>сложение. Необходимо, чтобы действия делались в правильном математическом порядке 
				// -->>
				if ((input[i] == '+') || (input[i] == '-')) {
					deistv_weight[kolvo_numbers] = 1; // запись в массив весов действий
				}
				else if ((input[i] == '*') || (input[i] == '/')) {
					deistv_weight[kolvo_numbers] = 2; // запись в массив весов действий
				}
				else if (input[i] == '^') {
					deistv_weight[kolvo_numbers] = 3; // запись в массив весов действий
				}
				// <<--
				kolvo_numbers++;
				buf = "";
			}
		}
		if (i == input.length() - 1) { //последнее число не запишется в буфер без принуждения. Здесь все также, как и в предыдущей записи числа
			up_numbers(buf, numbers, kolvo_numbers);
		}
	}
}
void schet_viragenia(int& kolvo_deistv, string& input, double*& numbers, string*& deistv, int*& deistv_weight, int& ch_n) { // метод для счета без каких-либо проверок
	for (int i = 0; i < kolvo_deistv; i++) { // поочередное выполнение действий
		int max_weidth = 0; // максимальные вес и индекс. По весу ищется индекс, который используется как индекс выполняющегося действия
		int max_index = 0;
		for (int j = 0; j < kolvo_deistv; j++) {
			if (deistv_weight[j] > max_weidth) { // если вес по индексу j больше, чем нынешний вес, индекс j становится максимальным индексом и вес индекса j становится максимальным весом
				max_weidth = deistv_weight[j];
				max_index = j;
			}
		}
		// следующите действия проходят по максимально найденному весу. Т.е. степень выполнится первее сложения или умножения. 
		// После действия и вес, и индекс стираются, из-за чего повторной сработки не будет
		if (deistv[max_index] == "+") { // сложение, последующая перезапись массивов -->> **
			numbers[max_index] = numbers[max_index] + numbers[max_index + 1];
			rewrite(max_index, kolvo_deistv, i, deistv, deistv_weight, numbers);// ** <<--
		}
		else if (deistv[max_index] == "-") { // вычитание, последующая перезапись массивов -->> **
			numbers[max_index] = numbers[max_index] - numbers[max_index + 1];
			rewrite(max_index, kolvo_deistv, i, deistv, deistv_weight, numbers);// ** <<--
		}
		else if (deistv[max_index] == "*") { // умножение, последующая перезапись массивов -->> **
			numbers[max_index] = numbers[max_index] * numbers[max_index + 1];
			rewrite(max_index, kolvo_deistv, i, deistv, deistv_weight, numbers);// ** <<--
		}
		else if (deistv[max_index] == "/") { //деление, последующая перезапись массивов -->> **
			if (numbers[max_index + 1] != 0) { // если второе НЕ 0, то все ок
				numbers[max_index] = numbers[max_index] / numbers[max_index + 1];
				rewrite(max_index, kolvo_deistv, i, deistv, deistv_weight, numbers);// ** <<--
			}
			else { // если второе 0, ломает цикл и меняет ch_n, сервак же выведет пользователю ошибку
				cout << "Ошибка. Деление на 0";
				ch_n = 0;
				break;
			}
		}
		else if (deistv[max_index] == "^") { // возведение в степень, последующая перезапись массивов --> **
			numbers[max_index] = pow(numbers[max_index], numbers[max_index + 1]);
			rewrite(max_index, kolvo_deistv, i, deistv, deistv_weight, numbers);// ** <<--
		}
	}
}
void privedenie(string& input) { //метод только для красивого вывода ответа без лишних нулей и запятых
	int check = 1;
	for (int i = input.length() - 1; check != 0; i--) { // с конца ответа перезаписывает 0 и запятые на пустышку, чтобы их видно не было, пока не встретит любое числo или запятую
		if (input[i] != '0' || input[i] == ',') {
			check = 0;
			if (input[i] == ',') {
				input[i] = ' ';
			}
		}
		else {
			input[i] = ' ';
		}
	}
}
double calculate(string& input, int& check_tochki, int& ch_n) { // метод для файла сервера, чтобы получить число ответа с помощью предыдущего метода
	string buf; //буфер для нахождения чисел
	int kolvo_minus = 0; // нужно только для счета удаления пробелов
	int kolvo_numbers = 0; //храни инфу о количестве чисел
	int kolvo_deistv = 0; //хранит инфу о количестве действий
	int check = 1; //для ошибок
	int check_minus = 0; //для конвертации первого числа, если оно отрицательное
	vector <int> buf_minus;
	for (int i = 0; i < input.length(); i++) { // уничтожение всех пробелов
		if ((input[i] == ' ') && (i != input.length() - 1)) { //при встрече пробела происходит перезапись, где индекс i, являющийся пробелом, будет удален
			rewrite1(i, input);
			kolvo_minus++; // в цикле проверки на буквы это число вычтется из длины строки, чтобы пробелы в конце строки не считывались
		}
	}
	check_errors(check, input, check_minus, buf_minus); // здесь идет проверка на правильность введения действий, например, ++ - ошибка
	if (check != 0) { //если ошибок нет, можно че-то делать
		for (int i = 0; i < input.length(); i++) { // подсчет количества действий
			if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/') || (input[i] == '^')) {
				kolvo_deistv++; // хранит в себе количество действий
			}
		}
		double* numbers = new double[kolvo_deistv + 1]; // создание массивов чисел, действий и весов этих действий
		string* deistv = new string[kolvo_deistv];
		int* deistv_weight = new int[kolvo_deistv];
		// работа кода:
		// строка делится на числа и действия следующим циклом
		// массив чисел - числовой, действий - строковый
		// также в отдельный массив записывается вес действия, массив - числовой
		// последним циклом в работе выбирается максимальный вес символа и его индекс
		// по этому индексу находится первое число и по индексу+1 следующее. В итоге имеем обычный пример из двух чисел и 1 действия между ними
		// выполняется действие, результат записывается в первое число, а вес, действие и число по индексу+1 удаляются с помощью перезаписи
		// Проделав такой цикл столько раз, сколько есть действий, останется 1 число, которое, очевидно, является ответом на весь пример
		
		//разделение по разным массивам чисел и действий
		delenie_deistv_ot_chisel(input, buf, numbers, deistv, deistv_weight, kolvo_numbers, check_tochki);
		if (check_tochki == 1) {
			if (check_minus == 1) { //конвертация первого числа, если стоит минус
				numbers[0] = numbers[0] * -1;
			}
			for (size_t i = 0; i < buf_minus.size(); i++) {// конвертация чисел, где после умножения шел минус
				numbers[buf_minus[i]] = numbers[buf_minus[i]] * -1;
			}
			schet_viragenia(kolvo_deistv, input, numbers, deistv, deistv_weight, ch_n);// непосредственное выполнение действий
			return numbers[0]; // Возврат единственного числа, являющегося ответом
		}
	}
}
