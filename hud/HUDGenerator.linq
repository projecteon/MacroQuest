<Query Kind="Statements" />

string macroHeader = @"[Leader] 
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

; |=========================================== NetBots ===>
";

var initalXAxis = 15;
var initalYAxis = 400;

string GenerateHud(string netbot, int counter) {
	var stringbuilder = new StringBuilder();
	
	var xAxis = initalXAxis;
	var yAxis = initalYAxis + 15*counter;
	
	// Name
	stringbuilder.AppendLine($"{netbot}_Name=3,{xAxis},{yAxis},000,255,0,${{If[${{NetBots[{netbot}].ID}},{netbot}, ]}}");
	xAxis += 50;
	
	// Level
	stringbuilder.AppendLine($"{netbot}_Level=3,{xAxis},{yAxis},000,255,0,${{If[${{NetBots[{netbot}].ID}},${{NetBots[{netbot}].Level}}, ]}}");
	xAxis += 20;
	
	// HP
	stringbuilder.AppendLine($"{netbot}_HP_Good=3,{xAxis},{yAxis},0,255,255,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}>=70,HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_HP_OK=3,{xAxis},{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].PctHPs}}>30&&${{NetBots[{netbot}].PctHPs}}<70),HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_HP_Bad=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}<=30,HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	xAxis += 50;

	// MP
	stringbuilder.AppendLine($"{netbot}_MP_Good=3,{xAxis},{yAxis},0,153,255,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}>=70,MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_MP_OK=3,{xAxis},{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}>30&&${{NetBots[{netbot}].PctMana}}<70),MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_MP_Bad=3,{xAxis},{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}<=30,MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	xAxis += 50;
	
	// Experience
	stringbuilder.AppendLine($"{netbot}_PctXp=3,{xAxis},{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctExp}}<100,XP:${{NetBots[{netbot}].PctExp}}%, ]}}, ]}}");
	stringbuilder.AppendLine($"{netbot}_PctXpMax=3,{xAxis},{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctExp}}>=100,XP: ∞, ]}}, ]}}");
	xAxis += 60;
	
	// Distance
	stringbuilder.AppendLine($"{netbot}_Dist=3,{xAxis},{yAxis},102,153,153,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}}&&${{NetBots[{netbot}].Zone}}==${{Zone.ID}},D:${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}},]}}");
	stringbuilder.AppendLine($"{netbot}_DistInf=3,{xAxis},{yAxis},102,153,153,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}}&&${{NetBots[{netbot}].Zone}}!=${{Zone.ID}},D: ∞,]}}");
	xAxis += 50;
	
	// Target
	stringbuilder.AppendLine($"{netbot}_Tar=3,{xAxis},{yAxis},255,51,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].TargetID}}, T:${{Spawn[${{NetBots[{netbot}].TargetID}}].Name}},]}},]}}");
	xAxis += 130;
	
	// CurrentMacro
	stringbuilder.AppendLine($"{netbot}_Mac=3,{xAxis},{yAxis},255,51,153,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].MacroName}}]}},${{NetBots[{netbot}].MacroName}},]}},]}}");
	xAxis += 30;
	
	// Current casting spell
	stringbuilder.AppendLine($"{netbot}_Cast=3,{xAxis},{yAxis},255,255,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].Casting}}]}},${{NetBots[{netbot}].Casting}},]}},]}}");
	xAxis += 40;
	
	// Combatstate
	//stringbuilder.AppendLine($"{netbot}_Cbt=3,{xAxis},{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].CombatState}}]}},${{NetBots[{netbot}].CombatState}},]}},]}}");
	
	return stringbuilder.ToString();
}

void GenerateGroupHud(IList<string> group) {
	for(var i = 0; i < group.Count(); i++) {
		Console.WriteLine(GenerateHud(group[i], i));
	}
}

var group1 = new List<string>() {
	"Eredhrin"
	,"Hamfast"
	,"Ithildin"
	,"Newt"
	,"Bill"
	,"Tedd"
};


var group2 = new List<string>() {
	"Regis"
	,"Aredhel"
	,"Freyja"
	,"Marillion"
	,"Ronin"
	,"Osiris"
};

Console.WriteLine(macroHeader);
GenerateGroupHud(group1);
initalYAxis = initalYAxis + group1.Count() * 15 + 30;
GenerateGroupHud(group2);
Console.WriteLine("; <===============================================================|");