class ARMST_USER_PLAYRADIO : ScriptedUserAction
{
	[Attribute("", UIWidgets.Auto)]
	protected ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
	
	[Attribute("0", UIWidgets.ComboBox, "Instrument type", "", ParamEnumArray.FromEnum(SCR_EInstrumentType) )]
	protected SCR_EInstrumentType m_eInstrumentType;
	
	[Attribute("", UIWidgets.Coords)]
	private vector m_vSoundOffset;
	
	protected AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	protected static ref ScriptInvokerInt2 s_onInstrumentPlayed;
	
	//------------------------------------------------------------------------------------------------
	
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_action_play_radio";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnActionStart(IEntity pUserEntity) 
	{
		//OnActionCanceled(GetOwner(), pUserEntity);
		if (s_onInstrumentPlayed)
			s_onInstrumentPlayed.Invoke(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity), m_eInstrumentType);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
				
		if (!m_AudioSourceConfiguration || !m_AudioSourceConfiguration.IsValid())
			return;
		
		SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(GetOwner(), m_AudioSourceConfiguration);
		if (!audioSource)
			return;
		
		vector mat[4];
		IEntity owner = GetOwner();
		owner.GetTransform(mat);		
		mat[3] = owner.CoordToParent(m_vSoundOffset);
					
		AudioSystem.TerminateSound(m_AudioHandle);
		soundManagerEntity.PlayAudioSource(audioSource, mat);			
		m_AudioHandle = audioSource.m_AudioHandle;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnActionCanceled(IEntity pOwnerEntity, IEntity pUserEntity)
	{		
		AudioSystem.TerminateSound(m_AudioHandle);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
				
		if (!m_AudioSourceConfiguration || m_AudioSourceConfiguration.m_sSoundProject == string.Empty)
			return;
		
		SCR_AudioSourceConfiguration audioSourceConfiguration = new SCR_AudioSourceConfiguration;
		audioSourceConfiguration.m_sSoundProject = m_AudioSourceConfiguration.m_sSoundProject;
		audioSourceConfiguration.m_eFlags = m_AudioSourceConfiguration.m_eFlags;
		audioSourceConfiguration.m_sSoundEventName = SCR_SoundEvent.SOUND_STOP_PLAYING;
				
		SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(pOwnerEntity, audioSourceConfiguration);
		if (!audioSource)
			return;
		
		vector mat[4];
		pOwnerEntity.GetTransform(mat);
		mat[3] = pOwnerEntity.CoordToParent(m_vSoundOffset);
					
		soundManagerEntity.PlayAudioSource(audioSource, mat);			
		m_AudioHandle = audioSource.m_AudioHandle;
	}
		
	//------------------------------------------------------------------------------------------------
	static ScriptInvokerInt2 GetOnInstrumentPlayed()
	{
		if (!s_onInstrumentPlayed)
			s_onInstrumentPlayed = new ScriptInvokerInt2();
		
		return s_onInstrumentPlayed;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_PLAYRADIO()
	{
		AudioSystem.TerminateSound(m_AudioHandle);
	}
};