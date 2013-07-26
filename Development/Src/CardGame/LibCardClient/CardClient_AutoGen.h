sigslot::signal0<> _OnPong;

void Pong(CCardClient* pClient)
{
	_OnPong();
}

