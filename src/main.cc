#include <global.h>
#include <checkArgs.hpp>
uint64_t sumasSecuenciales = 0;
uint64_t *sumasParalelas = nullptr;
uint64_t *arreglo_c = nullptr;
uint64_t *arreglo_p = nullptr;
uint64_t *arreglo_s = nullptr;
void sumaSecuencial(size_t pos,
				size_t beginIndex,
				size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sumasSecuenciales += arreglo_p[i];
	}
}
void sumaParalela(size_t pos,
				  size_t beginIndex,
				  size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sumasParalelas[pos] += arreglo_p[i];
	}
}

void llenadoAr(size_t beginIdx, size_t endIdx, uint32_t limiteInferior, uint32_t limiteSuperior, size_t Rnetype)
{
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

		case 1:
			arreglo_s[i] = random;
			break;
		}
	}
}

int main(int argc, char **argv)
{

	uint64_t totalElementos;
	uint64_t numThreads;
	uint32_t limiteInferior;
	uint32_t limiteSuperior;
	uint64_t sumaThreads = 0;
	std::vector<std::thread *> threads;
	std::vector<std::thread *> threads2;

	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	limiteInferior = argumentos->getArgs().limiteInferior;
	limiteSuperior = argumentos->getArgs().limiteSuperior;
	//--------------------------------------------------------llenado del arreglo en secuencia--------------------------------------------------------
	arreglo_s = new uint64_t[totalElementos];
	auto start = std::chrono::high_resolution_clock::now();
	llenadoAr(0, totalElementos, limiteInferior, limiteSuperior, 1);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSerial = elapsed.count();

	//--------------------------------------------------------llenado del arreglo paralelo---------------------------------------------------------

	arreglo_p = new uint64_t[totalElementos];
	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.push_back(new std::thread(llenadoAr,
										  i * (totalElementos) / numThreads,
										  (i + 1) * (totalElementos) / numThreads, limiteInferior, limiteSuperior, 0));
	}
	//-------------------------------------------------tiempo de llenado paralelo-------------------------------------------------------------------
	start = std::chrono::system_clock::now();
	for (auto &thrs : threads)
	{
		thrs->join();
	}
	end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> duration = end - start;
	auto totalTimefilled = duration.count();
	//------------------------------------------suma Threads---------------------------------------------------------------------------------------
	sumasParalelas = new uint64_t[totalElementos];
	for (size_t i = 0; i < numThreads; ++i)
	{

		threads2.push_back(new std::thread(sumaParalela,
										   i,
										   i * (totalElementos) / numThreads,
										   (i + 1) * (totalElementos) / numThreads));
	}
	//Tiempo de suma Threads
	start = std::chrono::system_clock::now();
	for (auto &thr : threads2)
	{
		thr->join();
	}
	end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> duration2 = end - start;
	auto totalTimeThread = duration2.count();
	//--------------------------------------------Tiempo de sumaSecuencial---------------------------------

	start = std::chrono::high_resolution_clock::now();
	llenadoAr(0, totalElementos, limiteInferior, limiteSuperior, 1);
	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSumSerial = elapsed.count();
	//--------------------------------------------suma en secuencia---------------------------------
	sumaSecuencial(0,0,totalElementos);
	
	//--------------------------------------------suma en paralelo---------------------------------

	for (size_t i = 0; i < numThreads; i++)
	{
		sumaThreads += sumasParalelas[i];
	}

	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads  : " << numThreads << std::endl;
	std::cout << "limite inferior: " << limiteInferior << std::endl;
	std::cout << "limite Superior: " << limiteSuperior << std::endl;
	std::cout << "==========Suma Total=================" << std::endl;
	std::cout << "Suma en Secuencial: " << sumasSecuenciales << std::endl;
	std::cout << "Suma en Paralelo: " << sumaThreads << std::endl;
	std::cout << "==========Tiempos de Llenado=========" << std::endl;
	std::cout << "TiempodeLLenadoSecuencial: " << totalTimeSerial << "[ms]" << std::endl;
	std::cout << "TiempodeLLenadoParalelo: " << totalTimefilled << "[ms]" << std::endl;
	std::cout << "SpeedUp Etapa de Llenado: " << (double)totalTimeSerial/totalTimefilled << std::endl;
	std::cout << "==========Tiempos de Sumado==========" << std::endl;
	std::cout << "TiempoDeSumadoSecuencial: " << totalTimeSumSerial << "[ms]" << std::endl;
	std::cout << "TiempoDeSumadoParalelo: " << totalTimeThread << "[ms]" << std::endl;
	std::cout << "SpeedUp Etapa de Suma: " << (double)totalTimeSumSerial/totalTimeThread << std::endl;
	return (EXIT_SUCCESS);
}