#include "pch.h"
#include "Connection.h"
#include "Datacenter.h"
#include "ConnectionManager.h"

using namespace Telegram::Api::Native;


Connection::Connection() :
	m_token(0),
	m_type(ConnectionType::Generic),
	m_currentNetworkType(ConnectionNeworkType::WiFi)
{
}

Connection::~Connection()
{
}

HRESULT Connection::RuntimeClassInitialize(Datacenter* datacenter, ConnectionType type)
{
	HRESULT result;
	ReturnIfFailed(result, MakeAndInitialize<Timer>(&m_reconnectionTimer, [&]
	{
		return Connect();
	}));

	m_datacenter = datacenter;
	m_type = type;

	return S_OK;
}

HRESULT Connection::get_Token(UINT32* value)
{
	if (value == nullptr)
	{
		return E_POINTER;
	}

	auto lock = m_criticalSection.Lock();

	*value = m_token;
	return S_OK;
}

HRESULT Connection::get_Datacenter(IDatacenter** value)
{
	if (value == nullptr)
	{
		return E_POINTER;
	}

	auto lock = m_criticalSection.Lock();
	return m_datacenter.CopyTo(value);
}

HRESULT Connection::get_Type(ConnectionType* value)
{
	if (value == nullptr)
	{
		return E_POINTER;
	}

	*value = m_type;
	return S_OK;
}

HRESULT Connection::get_CurrentNetworkType(ConnectionNeworkType* value)
{
	if (value == nullptr)
	{
		return E_POINTER;
	}

	auto lock = m_criticalSection.Lock();

	*value = m_currentNetworkType;
	return S_OK;
}

HRESULT Connection::OnEvent(PTP_CALLBACK_INSTANCE callbackInstance)
{
	auto lock = m_criticalSection.Lock();

	boolean socketClosed = false;
	HRESULT result = OnSocketEvent(callbackInstance, &socketClosed);

	if (!socketClosed)
	{
		SetThreadpoolWait(GetThreadpoolObjectHandle(), GetSocketEvent(), nullptr);
	}

	return result;
}

HRESULT Connection::Connect()
{
	HRESULT result;
	//auto lock = m_criticalSection.Lock();

	ComPtr<ConnectionManager> connectionManager;
	ReturnIfFailed(result, ConnectionManagerStatics::GetInstance(connectionManager));

	//I_WANT_TO_DIE_IS_THE_NEW_TODO("Implement connection start");

	boolean ipv6;
	ReturnIfFailed(result, connectionManager->get_IsIpv6Enabled(&ipv6));

	/*Datacenter::DatacenterEndpoint* endpoint;
	if (FAILED(result = m_datacenter->GetCurrentEndpoint(m_type, ipv6, &endpoint)) && ipv6)
	{
		ipv6 = false;
		ReturnIfFailed(result, m_datacenter->GetCurrentEndpoint(m_type, false, &endpoint));
	}
	else
	{
		return result;
	}

	ReturnIfFailed(result, m_reconnectionTimer->Stop());

	ReturnIfFailed(result, OpenSocket(endpoint->Address, endpoint->Port, ipv6));*/

	ReturnIfFailed(result, m_reconnectionTimer->Stop());

	{
		auto lock = m_criticalSection.Lock();
		ReturnIfFailed(result, ConnectSocket(L"172.217.23.68", 80, false));
	}


	ReturnIfFailed(result, connectionManager->get_CurrentNetworkType(&m_currentNetworkType));

	/*Sleep(10000);

	{
		auto lock = m_criticalSection.Lock();
		ReturnIfFailed(result, DisconnectSocket());
	}

	Sleep(10000);

	{
		auto lock = m_criticalSection.Lock();
		ReturnIfFailed(result, ConnectSocket(L"172.217.23.68", 80, false));
	}*/

	return S_OK;
}

HRESULT Connection::Reconnect()
{
	HRESULT result;
	ReturnIfFailed(result, Suspend());

	return Connect();
}

HRESULT Connection::Suspend()
{
	HRESULT result;
	auto lock = m_criticalSection.Lock();

	I_WANT_TO_DIE_IS_THE_NEW_TODO("Implement connection suspension");

	ReturnIfFailed(result, m_reconnectionTimer->Stop());

	return S_OK;
}

HRESULT Connection::OnSocketCreated()
{
	auto threadpoolObjectHandle = EventObjectT::GetThreadpoolObjectHandle();
	if (threadpoolObjectHandle == nullptr)
	{
		return E_NOT_VALID_STATE;
	}

	SetThreadpoolWait(threadpoolObjectHandle, GetSocketEvent(), nullptr);
	return S_OK;
}

HRESULT Connection::OnSocketConnected()
{
	I_WANT_TO_DIE_IS_THE_NEW_TODO("Implement socket connected event handling");

	return S_OK;
}

HRESULT Connection::OnDataReceived()
{
	I_WANT_TO_DIE_IS_THE_NEW_TODO("Implement socket connection data received handling");

	return S_OK;
}

HRESULT Connection::OnSocketDisconnected()
{
	I_WANT_TO_DIE_IS_THE_NEW_TODO("Implement socket disconnected event handling");

	return S_OK;
}

HRESULT Connection::OnSocketClosed(int wsaError)
{
	I_WANT_TO_DIE_IS_THE_NEW_TODO("Implement socket connection closed handling");

	auto threadpoolObjectHandle = EventObjectT::GetThreadpoolObjectHandle();
	if (threadpoolObjectHandle == nullptr)
	{
		return E_NOT_VALID_STATE;
	}

	SetThreadpoolWait(threadpoolObjectHandle, nullptr, nullptr);
	return S_OK;
}