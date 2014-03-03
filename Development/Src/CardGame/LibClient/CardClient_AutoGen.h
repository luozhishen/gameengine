sigslot::signal0<> _OnPong;
sigslot::signal3<_U32, const CARD_AVATAR_DESC*, _U32> _OnGetAvatarListCallback;
sigslot::signal1<_U32> _OnCreateAvatarCallback;
sigslot::signal1<_U32> _OnEnterGameCallback;
sigslot::signal1<_U32> _OnLeaveGameCallback;

void Pong()
{
	_OnPong();
}
void GetAvatarListCallback(_U32 errcode, const CARD_AVATAR_DESC* arr, _U32 count)
{
	_OnGetAvatarListCallback(errcode, arr, count);
}
void CreateAvatarCallback(_U32 errcode)
{
	_OnCreateAvatarCallback(errcode);
}
void EnterGameCallback(_U32 errcode)
{
	_OnEnterGameCallback(errcode);
}
void LeaveGameCallback(_U32 errcode)
{
	_OnLeaveGameCallback(errcode);
}

