var initalYAxis = 400;
string GenerateHud(string netbot, int counter) {
	var stringbuilder = new StringBuilder();
	
	var yAxis = initalYAxis + 15*counter;
	stringbuilder.AppendLine($"{netbot}_Name=3,15,{yAxis},000,255,000,${{If[${{NetBots[{netbot}].ID}},{netbot},NA]}}");
	stringbuilder.AppendLine($"{netbot}_HP=3,65,{yAxis},000,255,255,HP:${{If[${{NetBots[{netbot}].ID}},${{NetBots[{netbot}].PctHPs}},]}}%");
	stringbuilder.AppendLine($"{netbot}_Mana=3,115,{yAxis},0,153,255,MP:${{If[${{NetBots[{netbot}].ID}},${{NetBots[{netbot}].PctMana}},]}}%");
	stringbuilder.AppendLine($"{netbot}_Dist=3,165,{yAxis},102,153,153,Dist:${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}]}}, ${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance}},]}},]}}");
	stringbuilder.AppendLine($"{netbot}_Tar=3,225,{yAxis},255,51,0,Tar:${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].TargetID}}, ${{Spawn[${{NetBots[{netbot}].TargetID}}].CleanName}},]}},]}}");
	stringbuilder.AppendLine($"{netbot}_Cast=3,355,{yAxis},255,0,102,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].Casting}}]}},${{NetBots[{netbot}].Casting}},]}},]}}");
	
	return stringbuilder.ToString();
}

string GenerateHud2(string netbot, int counter) {
	var stringbuilder = new StringBuilder();
	
	var yAxis = initalYAxis + 15*counter;
	stringbuilder.AppendLine($"{netbot}_Name=3,15,{yAxis},000,255,0,${{If[${{NetBots[{netbot}].ID}},{netbot}, ]}}");
	stringbuilder.AppendLine($"{netbot}_Level=3,65,{yAxis},000,255,0,${{If[${{NetBots[{netbot}].ID}},${{NetBots[{netbot}].Level}}, ]}}");
	
	// HP
	stringbuilder.AppendLine($"{netbot}_HP_Good=3,85,{yAxis},0,255,255,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}>=70,HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_HP_OK=3,85,{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].PctHPs}}>30&&${{NetBots[{netbot}].PctHPs}}<70),HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_HP_Bad=3,85,{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}<=30,HP:${{NetBots[{netbot}].PctHPs}}%,]}},]}}");

	// MP
	stringbuilder.AppendLine($"{netbot}_MP_Good=3,135,{yAxis},0,153,255,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}>=70,MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_MP_OK=3,135,{yAxis},255,204,0,${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}>30&&${{NetBots[{netbot}].PctMana}}<70),MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	stringbuilder.AppendLine($"{netbot}_MP_Bad=3,135,{yAxis},255,102,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].MaxMana}}>0&&${{NetBots[{netbot}].PctMana}}<=30,MP:${{NetBots[{netbot}].PctMana}}%,]}},]}}");
	
	// Exp
	stringbuilder.AppendLine($"{netbot}_PctXp=3,185,{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctExp}}<100,XP:${{NetBots[{netbot}].PctExp}}%, ]}}, ]}}");
	stringbuilder.AppendLine($"{netbot}_PctXpMax=3,185,{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctExp}}>=100,XP: ∞, ]}}, ]}}");
	
	// Dist
	stringbuilder.AppendLine($"{netbot}_Dist=3,245,{yAxis},102,153,153,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}}&&${{NetBots[{netbot}].ID}}==${{Zone.ID}},D:${{Spawn[id ${{NetBots[{netbot}].ID}} ].Distance3D}},]}}");
	stringbuilder.AppendLine($"{netbot}_Dist=3,245,{yAxis},102,153,153,${{If[${{NetBots[{netbot}].ID}}&&${{NetBots[{netbot}].ID}}!=${{Me.ID}}&&${{NetBots[{netbot}].ID}}!=${{Zone.ID}},D: ∞,]}}");
	
	// Div
	stringbuilder.AppendLine($"{netbot}_Tar=3,295,{yAxis},255,51,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].TargetID}}, T:${{Spawn[${{NetBots[{netbot}].TargetID}}].Name}},]}},]}}");
	stringbuilder.AppendLine($"{netbot}_Mac=3,425,{yAxis},255,51,153,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].MacroName}}]}},${{NetBots[{netbot}].MacroName}},]}},]}}");
	stringbuilder.AppendLine($"{netbot}_Cast=3,455,{yAxis},255,255,0,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].Casting}}]}},${{NetBots[{netbot}].Casting}},]}},]}}");
	//stringbuilder.AppendLine($"{netbot}_Cbt=3,495,{yAxis},102,255,204,${{If[${{NetBots[{netbot}].ID}},${{If[${{Bool[${{NetBots[{netbot}].CombatState}}]}},${{NetBots[{netbot}].CombatState}},]}},]}}");
	
	return stringbuilder.ToString();
}

void GenerateGroupHud(IList<string> group) {
	for(var i = 0; i < group.Count(); i++) {
		Console.WriteLine(GenerateHud2(group[i], i));
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
GenerateGroupHud(group1);

initalYAxis = initalYAxis + group1.Count() * 15 + 30;
var group2 = new List<string>() {
	"Regis"
	,"Aredhel"
	,"Freyja"
	,"Boombero"
	,"Marillion"
	,"Ronin"
};
GenerateGroupHud(group2);


/*
stringbuilder.AppendLine($"{netbot}_HP_Good =3,3,{yAxis},110,0,255,0,HP:${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}>=70,${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
stringbuilder.AppendLine($"{netbot}_HP_OK   =3,3,{yAxis},110,255,255,0,HP:${{If[${{NetBots[{netbot}].ID}},${{If[(${{NetBots[{netbot}].PctHPs}}>30&&${{NetBots[{netbot}].PctHPs}}<70),${{NetBots[{netbot}].PctHPs}}%,]}},]}}");
stringbuilder.AppendLine($"{netbot}_HP_Bad  =3,3,{yAxis},110,255,0,0,HP:${{If[${{NetBots[{netbot}].ID}},${{If[${{NetBots[{netbot}].PctHPs}}<=30,${{NetBots[{netbot}].PctHPs}}%,]}},]}}");


*/