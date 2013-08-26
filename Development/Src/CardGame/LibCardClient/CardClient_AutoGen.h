sigslot::signal0<> _OnPong;
sigslot::signal3<_U32, const CARD_AVATAR_DESC*, _U32> _OnGetAvatarListCallback;
sigslot::signal1<_U32> _OnCreateAvatarCallback;
sigslot::signal1<_U32> _OnEnterGameCallback;
sigslot::signal1<const CARD_AVATAR&> _OnSyncAvatar;
sigslot::signal1<const CARD_AVATAR_OWNOBJ&> _OnSyncAvatarOwnObj;
sigslot::signal0<> _OnSyncCompleted;
sigslot::signal1<_U32> _OnLeaveGameCallback;

void Pong(CCardClient* pClient)
{
	_OnPong();
}
void GetAvatarListCallback(CCardClient* pClient, _U32 errcode, const CARD_AVATAR_DESC* arr, _U32 count)
{
	_OnGetAvatarListCallback(errcode, arr, count);
}
void CreateAvatarCallback(CCardClient* pClient, _U32 errcode)
{
	_OnCreateAvatarCallback(errcode);
}
void EnterGameCallback(CCardClient* pClient, _U32 errcode)
{
	_OnEnterGameCallback(errcode);
}
void SyncAvatar(CCardClient* pClient, const CARD_AVATAR& avatar)
{
	_OnSyncAvatar(avatar);
}
void SyncAvatarOwnObj(CCardClient* pClient, const CARD_AVATAR_OWNOBJ& ownobj)
{
	_OnSyncAvatarOwnObj(ownobj);
}
void SyncCompleted(CCardClient* pClient)
{
	_OnSyncCompleted();
}
void LeaveGameCallback(CCardClient* pClient, _U32 errcode)
{
	_OnLeaveGameCallback(errcode);
}

