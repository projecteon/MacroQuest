string macroHeader = @"[Elements] 
; |====================================== Target Class and lvl ===>
TargetName=3,15,285,000,255,000,${If[${Target.ID},${Target.CleanName},]}
; <===============================================================|
; |====================================== Target Class and lvl ===>
TargetLevel=3,15,300,000,255,000,${If[${Target.ID},${Target.Level},]}
TargetClass=3,35,300,000,255,255,${If[${Target.ID},${Target.Class},]}
TargetID=3,115,300,255,125,008,${If[${Target.ID},[ID: ${Target.ID}],]}
; <===============================================================|

; |=========================================== Target Distance ===>
TargetDist_Text=3,15,315,000,255,255,${If[${Target.Name.NotEqual[""NULL""]},Distance:,]}
TargetDist_Info=3,65,315,000,255,000,${Target.Distance3D}
; <===============================================================|

[TheNetBots] 
; |=========================================== NetBots ===>
";

var initalXAxis = 15;
var initalYAxis = 400;

string GenerateHud((string netbot, bool petuser) groupMember, int counter) {
	var netbot = groupMember.netbot;
	var stringbuilder = new StringBuilder();
	
	var xAxis = initalXAxis;
	var yAxis = initalYAxis + 15*counter;
	
	// Name
	stringbuilder.AppendLine($"{netbot}_Name=3,{xAxis},{yAxis},000,255,0,${{If[${{NetBots[{netbot}].ID}},{netbot}, ]}}");
	xAxis += 70;
	
	// Level
	stringbuilder.AppendLine($"{netbot}_Level=3,{xAxis},{yAxis},000,255,0,${{If[${{NetBots[{netbot}].ID}},${{NetBots[{netbot}].Level}}, ]}}");
	xAxis += 20;
	
	xAxis = AppendHpHUD(stringbuilder, netbot, xAxis, yAxis);
	xAxis = AppendManaHUD(stringbuilder, netbot, xAxis, yAxis);
	xAxis = AppendXPHUD(stringbuilder, netbot, xAxis, yAxis);
	xAxis = AppendDistanceHUD(stringbuilder, netbot, xAxis, yAxis);
	xAxis = AppendTargetHUD(stringbuilder, netbot, xAxis, yAxis);
	if(groupMember.petuser) {
		xAxis = AppendPetHpHUD(stringbuilder, netbot, xAxis, yAxis);
	}
	
	xAxis = AppendCastingHUD(stringbuilder, netbot, xAxis, yAxis);
	
	return stringbuilder.ToString();
}

int AppendHpHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_HP_Good=3,{xAxis},{yAxis},0,255,255,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}>=70,HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_HP_OK=3,{xAxis},{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].PctHPs}}>30&&${{NetBots[{netbot}].PctHPs}}<70),HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_HP_Bad=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}<=30,HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	return xAxis + 50;
}

int AppendManaHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_MP_Good=3,{xAxis},{yAxis},0,153,255,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}>=70,MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_MP_OK=3,{xAxis},{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}>30&&${{NetBots[{netbot}].PctMana}}<70),MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_MP_Bad=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}<=30,MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	return xAxis + 50;
}

int AppendXPHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_PctXp=3,{xAxis},{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctExp}}<100,XP:${{NetBots[{netbot}].PctExp}}%, ]}}, ]}}");
	stringbuilder.AppendLine($"{netbot}_PctXpMax=3,{xAxis},{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctExp}}>=100,XP: ∞, ]}}, ]}}");
	return xAxis + 60;
}

int AppendDistanceHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_Dist=3,{xAxis},{yAxis},102,153,153,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}}&&${{NetBots[{netbot}].InZone}}&&${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}}=<200,D:${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}},]}}");
	stringbuilder.AppendLine($"{netbot}_DistWarn=3,{xAxis},{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}},${{If[${{NetBots[{netbot}].InZone}}&&${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}}>200&&${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}}<=500,D:${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}},]}},]}}");
	stringbuilder.AppendLine($"{netbot}_DistErr=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}},${{If[${{NetBots[{netbot}].InZone}}&&${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}}>500,D:${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}},]}},]}}");
	stringbuilder.AppendLine($"{netbot}_DistDiffZone=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}},${{If[${{NetBots[{netbot}].InZone}},,Z:${{Zone[${{NetBots[{netbot}].Zone}}].ShortName}}]}},]}}");
	return xAxis + 50;
}

int AppendTargetHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_Tar=3,{xAxis},{yAxis},255,51,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].TargetID}}, T:${{Spawn[${{NetBots[{netbot}].TargetID}}].Name}},]}},]}}");
	return xAxis + 130;
}

int AppendCurrentMacroHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_Mac=3,{xAxis},{yAxis},255,51,153,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].MacroName}}]}},${{NetBots[{netbot}].MacroName}},]}},]}}");
	return xAxis + 30;
}

int AppendCastingHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_Cast=3,{xAxis},{yAxis},255,255,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].Casting}}]}},${{NetBots[{netbot}].Casting}},]}},]}}");
	return xAxis + 40;
}

int AppendCombatStateHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_Cbt=3,{xAxis},{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].CombatState}}]}},${{NetBots[{netbot}].CombatState}},]}},]}}");
	return xAxis + 30;
}

int AppendPetHpHUD(StringBuilder stringbuilder, string netbot, int xAxis, int yAxis) {// HP
	stringbuilder.AppendLine($"{netbot}_Pet_HP_Good=3,{xAxis},{yAxis},0,255,255,${{If[${{NetBots[{netbot}].PetID}},${{If[${{NetBots[{netbot}].PetHP}}>=70,HP:${{NetBots[{netbot}].PetHP}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_Pet_HP_OK=3,{xAxis},{yAxis},255,204,0,${{If[${{NetBots[{netbot}].PetID}},${{If[(${{NetBots[{netbot}].PetHP}}>30&&${{NetBots[{netbot}].PetHP}}<70),HP:${{NetBots[{netbot}].PetHP}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_Pet_HP_Bad=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].PetID}},${{If[${{NetBots[{netbot}].PetHP}}<=30,HP:${{NetBots[{netbot}].PetHP}}%,]}},]}}");
	return xAxis + 50;
}


void GenerateGroupHud(IList<(string netbot, bool petuser)> group) {
	for(var i = 0; i < group.Count(); i++) {
		Console.WriteLine(GenerateHud(group[i], i));
	}
}

var group1 = new List<(string netbot, bool petuser)>() {
	("Eredhrin", false)
	,("Hamfast", false)
	,("Ithildin", false)
	,("Newt", false)
	,("Bill", true)
	,("Tedd", true)
};

var group2 = new List<(string netbot, bool petuser)>() {
	("Regis", false)
	,("Aredhel", false)
	,("Freyja", true)
	,("Marillion", false)
	,("Ronin", false)
	,("Osiris", true)
};

var group3 = new List<(string netbot, bool petuser)>() {
	("Sheperd", false)
	,("Renaissance", false)
	,("Soundgarden", false)
	,("Inara", false)
	,("Milamber", false)
	,("Tyrion", false)
};

var group4 = new List<(string netbot, bool petuser)>() {
	("Lolth", false)
	,("Vierna", false)
	,("Eilistraee", false)
	,("Valsharess", false)
	,("Araushnee", false)
	,("Genesis", false)
};

Console.WriteLine(macroHeader);
GenerateGroupHud(group1);
initalYAxis = initalYAxis + group1.Count() * 15 + 20;
GenerateGroupHud(group2);
initalYAxis = initalYAxis + group2.Count() * 15 + 20;
GenerateGroupHud(group3);
initalYAxis = initalYAxis + group3.Count() * 15 + 20;
GenerateGroupHud(group4);
Console.WriteLine("; <===============================================================|");