
modded class SCR_BaseGameMode : BaseGameMode
{

	void Ping()
	{
		Print("Ping from gamemode");
	}
	IEntity sex; 
	
	 float f_fPrice;
	void SpawnEnty(string entity)
	  {      
	        vector upOffset = "0 5 0";
			vector pos = "0 0 0";
	         auto params = new EntitySpawnParams();
	        vector startOffset = "0 0 0";
	        TraceParam param = new TraceParam();
	        float maxLength = 10;
	        param.Start = pos + startOffset;
	          params.TransformMode = ETransformMode.WORLD;
		
	        Resource resource = Resource.Load(entity);
	        if (resource.IsValid())
	        {
	        	sex =  GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
	         	Print("Done spawn");
	        }
	  }  
	
	
  [RplRpc(RplChannel.Reliable, RplRcver.Server)]
  void Rpc_SpawnEntyBc(string entity)
  {
    SpawnEnty(entity);
  }
}

class SCR_ArmstGameModeClass: SCR_BaseGameModeClass{};
class SCR_ArmstGameMode : SCR_BaseGameMode
{
	void Ping2()
	{
		Print("Ping from gamemode");
	}
	IEntity sex2; 
	
	void SpawnEnty2(string entity)
	  {      
	        vector upOffset = "0 5 0";
			vector pos = "0 0 0";
	         auto params = new EntitySpawnParams();
	        vector startOffset = "0 0 0";
	        TraceParam param = new TraceParam();
	        float maxLength = 10;
	        param.Start = pos + startOffset;
	          params.TransformMode = ETransformMode.WORLD;
		
	        Resource resource = Resource.Load(entity);
	        if (resource.IsValid())
	        {
	        	sex =  GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
	         	Print("Done spawn");
	        }
	  }  
	
		
	
  [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
  void Rpc_SpawnEntyBc2(string entity)
  {
    SpawnEnty(entity);
  }
}