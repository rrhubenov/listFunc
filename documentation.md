# Интерпретатор на функционалния език ListFunc

Проект по Структури от данни и програмиране, ФМИ 2022
Интерпретаторът има за цел да може да работи в интерактивен режим под формата на REPL(read-eval-print loop) и да оценява изрази от файлове.
Езикът поддържа:
- 2 вида литерала: Double, List
- Деклариране на функции
- Рекурсия
- 10+ вградени функции
- четене от файлове, когато името на файла се подаде като 1-ви аргумент
## Общ поглед над архитектурата на интерпретатора

Интерпреторът работи на 3 основни стъпки.
1. Lexer
    В тази стъпка, входният поток от данни бива разбит на tokens(лексеми), които представляват разбираеми думи за нашия език. Например string-а "nand(#0,1)" бива разбит на думите "nand", "(", "#0", ",", 1, ")". 
2. Parser (Parsing)
    След като превърнем входния поток в списък от думи, които езикът ни познава, трябва да го превърнем в синтактично дърво, което отговаря на expression, който е зададен от потребителя. Целта е да представим входа спрямо граматиката, която сме дефинирали за езика. В примера с nand(#0, 1), това отговаря на правилото:
<function-call> ::= <function-name>([<expression>, ...])
3. Interpreting
Накрая, след като имаме генерираното синтактично дърво, можем да го обхождаме и да оценяваме всеки един от върховете му. Тъй като в нашия език няма side-effects, всяко оценяване води единствено до литерал (Double или List). За да оценим даден връх оценяваме неговите деца, докато не стигнем до листа, за които знаем как да оценим.
Например expression от вид Double винаги се оценява до самия него.
Докато expression от вид function-call се оценява като първо оценим аргументите му и след това оценим тялото на функцията чрез получените оценки на аргументите.
В зависимост от това дали оценяването е стриктно или линиво, може да варира кога биват оценявани аргументите на функцията.

## Lexer + Parser

В своята имплемнетация на езика, съм решил стъпката на lexing и parsing да бъде комбинирана.
Докато lexer-ът разбива на думи входният поток, parser-ът се опитва да изгражда Expression върховете на синтактичното дърво без да чака lexer-ът да е приключил с целият поток. Когато lexer-ът разпознае, че познатата дума е идентификатор, то знаем, че това трябва да бъде или декларация на функция, или извикване на функция. Поглеждайки следващата дума дали е "->" или "(", parser-ът веднага може да различи двете. Когато това се случи, parser-ът рекурсивно се опитва да parse-не и остатъка от потока, използвайки lexer-a, докато не стигне до ")", в случая на function-call "\n", в случая на декларация на функция.
Освновните функции тук са "getTok", който прочита следващия char от потока и се опитва да открие кой е следващия token, и "parseExpression", който спрямо последно прочетения токен се опитва да parse-не дадения тип expression, използвайки lexer-a.
Този тип parser, който рекурсивно се опитва да генерира expression-и спрямо вида токени, се нарича recursive-descent parser.
Тук имаме един абстрактен Expression клас, който бива наследяван от:
- FunctionCallExpr
- NumExpr
- ParamArgExpr
- FunctionDefinitionExpr
- ListExpr

Всеки един от класовете пази списък от expression-ите, от които зависи.

## Interpreter

За да бъде интерпретацията коректна имаме нужда от място, в което да пазим декларираните функции, които се извикват.
Имаме две структури, които пазят декларирани функции.
unordered_map<String, (Value*)(function*)(vector<Expression>)> nativeFunctions.
Той пази function pointer към native C++ функция като nand, която е дефинирана директно в езика.
Environment global, който пази подобен unordered_map, но вместо function pointers, пази телата на дефинираните от потребителя функции като Expression-и.

При интерпретацията имаме отделно правило за всеки вид expression.
1. NumExpr върховете биват оценени до самата си стойност
2. ParamArgExpr не изисква да има интерпретация (Обяснение за това по-надолу)
3. FunctionDefinitionExpr добавя към global структурата нов expression и с него свързва името на функцията.
4. ListExpr първо оценяваме елементите му и след това оценяваме списъка с получените литерали.
5. FunctionCallExpr първо се проверява дали фунцкията e native или дефинирана от потребителя. Ако функцията е native, намираме function pointer-а й и я извикваме като й подаваме списък от подадените аргументи. Ако функцията не е native, първо намираме expression-а, който отговаря на тялото на функцията, която се вика и след това правим субституция на всички ParamArgExpr с подадените аргументи. Ако ParamArgExpr е #0, то той бива заместен с първия от подадените аргументи. Това силно улеснява работата ни, тъй като няма нужда да пазим стек или контекст, от който да оценяваме всеки път стойност да индивидуален параметър.

След като интерпретацията приключи, резултата е винаги литерал, който принтираме на стандартния изход.


