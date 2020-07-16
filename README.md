## TSSOO-taller02

##### Raúl  Ignacio Valenzuela Cinto - Raul.valenzuela@alumnos.uv.cl

###  1. Diseño de la solución  
El diseño de este taller se basa en la creación de un código el cual consistirá de 2 módulos,primeramente se tendrá que verificar si los parámetros ingresados son realmente funcionales, luego estos módulos tendrán que ser, uno de llenado de un arreglo en paralelo como se muestra en la siguiente figura.
 ![modulo1](http://imgfz.com/i/0L8PgWO.png)
 
 luego de esto se llevará a cabo la suma de este arreglo paralelamente como se muestra en la siguiente figura.
![modulo2](http://imgfz.com/i/PdfDlY7.png)

Terminando así con los módulos solicitados y continuando luego con pruebas de desempeño para analizar los datos entregados.
###   2. Estructura del código 
#### 2.1 Parte de verificación de datos  
Primero que nada se verificarán los datos ingresados como parámetros, si estos cumplen la forma de uso se llevará a cabo el código realizado dando así parámetros para que este haga los módulos antes mencionados, en cambio si los parámetros ingresados estaban erróneos o simplemente no existían, este mandaría la forma de uso como mensaje para entender la forma correcta de utilización. 
para esta parte, se tuvo que editar el archivo `checkArgs` para poder realizar la confirmación de parametros y la validación de estos. 
#### 2.2 llenado de el arreglo
para el llenado del arreglo se creo una forma thread safe para el llenado de forma paralela, en la cual se creo un arreglo que contenia los numeros random dados por rangos que se dieron en los parametros de entrada como se muestra a continuación. 
```
std::random_device device;
std::mt19937 rng(device());
std::uniform_int_distribution<uint32_t> nRandom(limiteInferior, limiteSuperior);
for (size_t i = beginIdx; i < endIdx; ++i)
{
uint32_t random = nRandom(rng);
switch (Rnetype)
{
case 0:
arreglo_p[i] = random;
break;
```
tomando los datos de este nuevo arreglo se pudo llenar otro arreglo de forma paralela gracias a la funcion ``push_back`, para luego tomar el desempeño de esta como se muestra a continuacion.
```
arreglo_p = new uint64_t[totalElementos];
	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.push_back(new std::thread(llenadoAr,
										  i * (totalElementos) / numThreads,
										  (i + 1) * (totalElementos) / numThreads, limiteInferior, limiteSuperior, 0));
	}
```
#### 2.3 sumado del arreglo 
para la suma paralela se creo una funcion que hicera esta tomando los datos que se encuentran en el arreglo ordenado paralelamente y luego sumarlo de la siguiente forma  
```
void sumaParalela(size_t pos,
				  size_t beginIndex,
				  size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sumasParalelas[pos] += arreglo_p[i];
	}
}
```
luego de hacerlo se guarda en una variable para después mostrar la suma en pantalla de la siguiente forma.
```
for (size_t i = 0; i < numThreads; i++)
	{
		sumaThreads += sumasParalelas[i];
	}
 std::cout << "Suma en Paralelo: " << sumaThreads << std::endl;
```
