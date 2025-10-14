/*
*
* My ImGui 
* 
* By: Nerostav "NERO" Kuznetsov
* 
* [!] Este programa é um software livre: você pode redistribuí-lo e/ou modificá-lo como quiser.
* 
*/

// ➡️ Reduz o tamanho do cabeçalho <Windows.h>, excluindo partes desnecessárias da API do Windows.
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h> 

#include <thread>
#include <cstdint>

// ➡️ Essa função é o ponto inicial onde você vai colocar o código que quer rodar dentro do processo alvo 
// ➡️ (por exemplo, inicializar um menu ImGui, hookar funções, alterar memória, etc).
// ➡️ Ela recebe como parâmetro o HMODULE(handle da DLL carregada).
void Setup(const HMODULE instance)
{

}

// Ponto de entrada da DLL.
// Essa função é chamada automaticamente pelo sistema quando a DLL é carregada ou descarregada.
BOOL WINAPI DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)

{
	if (reason == DLL_PROCESS_ATTACH)
	{
		// ➡️ Desativa notificações de criação e destruição de threads (THREAD_ATTACH/DETACH).
		// ➡️ Isso evita overhead e possíveis deadlocks em alguns casos — boas práticas para  DLLs injetadas :D 
		DisableThreadLibraryCalls(instance);
		// ➡️ Cria uma nova thread para rodar a função Setup.
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), 
			instance,
			0,
			nullptr
		);
		if (thread)
			// ➡️ Fecha o handle da thread (mas não a thread em si — ela continua rodando).
			// ➡️ Isso é apenas limpeza de recursos.
			CloseHandle(thread);
	}
	return TRUE;
}