modded class SCR_ChimeraCharacter: ChimeraCharacter
{


	//------------------------------------------------------------------------------------------------
	void ArmstCameraShake(float linearMagnitude, float angularMagnitude, float inTime, float sustainTime, float outTime)
	{
		Rpc(RpcDo_ArmstCameraShake, linearMagnitude,  angularMagnitude,  inTime,  sustainTime,  outTime);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_ArmstCameraShake(float linearMagnitude, float angularMagnitude, float inTime, float sustainTime, float outTime  )
	{
		//static void AddCameraShake(float linearMagnitude = 1.0, float angularMagnitude = 1.0, float inTime = 0.01, float sustainTime = 0.1, float outTime = 0.24)
		SCR_CameraShakeManagerComponent.AddCameraShake(linearMagnitude, angularMagnitude, inTime, sustainTime, outTime);
		GetGame().GetInputManager().SetGamepadRumble(0, 0.5, 0.5, 0, 0, 500);
	}
}