/*
*
* NK_ImGui - Nerostav Kuznetsov (ImGui Menu DXD9)
* 
* By: Nerostav "NERO" Kuznetsov
* 
* Criei um menu ImGui interno universal que funcionará para qualquer jogo DirectX9 e 
* NUNCA! Ele funciona criando um .dll que, após precisará ser atualizado a injeção,
* cria uma janela temporária sobre o jogo que usamos para obter o dispositivo DirectX9 do jogo.
* Usando esse dispositivo, podemos conectar EndScene e Reset (duas funções importantes do DirectX)
* com a biblioteca MinHook. Em seguida, configuramos o contexto ImGui e renderizamos nosso menu no EndScene.
* Tudo isso está reunido em alguns arquivos com um manipulador de exceções e mecanismos perfeitos de descarregamento
* de módulos. Espero que gostem!
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