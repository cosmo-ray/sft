let lfrm = [12,21;22,36;38,43;44,48;50,55;57,62;63,73;74,86;87,103];;
let llyr = ["too";"ku ";"ki";"ra";"me";"ku ";"ho";"shi ";"ni"];;
let trim s = if s.[String.length s - 1]='\r' then String.sub s 0 (String.length s - 1) else s;;

let frametocs frame fps = int_of_float(float_of_int(frame) *. 100. /. fps);;

let lfrmtolcs l fps = List.map (fun (x,y)->frametocs x fps,frametocs y fps) l;;

let listesecondes = lfrmtolcs lfrm;;

let extractinfo l =
  let sta,_ = List.hd l and _,fin = List.hd (List.rev l) in
  let l1 = List.map fst l in 
  let rec aux l = match l with
    |x::y::q->(y-x)::aux (y::q)
    |x::[]->[fin-x]
    |_->failwith "extractinfo liste vide" in
  sta,fin,List.rev (aux l1)
;;

let syllabe d s = "{\\k"^string_of_int(d)^"}"^s;;

let sur2 n = (if n<10 then "0" else "")^string_of_int(n);;
let inttohour n = let cs = n mod 100 in let tts = n/100 in let s = tts mod 60 in let ttm = tts / 60 in let h = ttm / 60 in let m = ttm mod 60 in
   string_of_int(h)^":"^sur2 m^":"^sur2 s^"."^sur2 cs;;

let phrase2string lfrm llyr fps = 
  let aux lyr oth = let l = List.combine lyr oth in List.fold_left (fun a->fun (b,c)->a^(syllabe c b)) "" l in
  let sta,fin,oth2 = extractinfo (lfrmtolcs lfrm fps) in let oth = List.rev oth2 in
  "Dialogue: 0,"^inttohour sta^","^inttohour fin^",Default,,0,0,0,,"^aux llyr oth
;;


let file2list f = let r = ref [] in let a = open_in f in let _ = try while true do r:=(input_line a)::(!r) done with _->close_in a in List.rev (!r);;

let frm1line ss = let s = trim ss in let i = String.index s ' ' in int_of_string(String.sub s 0 i),int_of_string(String.sub s (i+1) (String.length s - i - 1));;

let lyr1line s = let aux1 s = let i = String.index s '&' in String.sub s 0 i,String.sub s (i+1) (String.length s - i - 1) in
		 let rec aux2 current res = try let a,b=aux1 current in aux2 b (a::res) with _->current::res in
		 List.tl (List.rev (aux2 s []))
;;
let decoupephrases l l2 = 
  let ll = List.map List.length l2 in
  let rec aux l n ln r lr = if n=0 then if ln=[] then List.rev(List.rev(r)::lr) else (aux l (List.hd ln) (List.tl ln) [] (List.rev(r)::lr)) else aux (List.tl l) (n-1) ln (List.hd l::r) lr in
  aux l (List.hd ll) (List.tl ll) [] []
;;

let toy2ass lyrFile frmFile fps = 
  let lyrics = List.filter (fun x->x<>[]) (List.map lyr1line (file2list lyrFile)) in
  let frames = decoupephrases (List.map frm1line (file2list frmFile)) (lyrics) in
  let mix = List.combine lyrics frames in
  List.map (fun (a,b)->phrase2string b a fps) mix
;;

let assHeader =
["[Script Info]";
"Title: Default Aegisub file";
"ScriptType: v4.00+";
"WrapStyle: 0";
"ScaledBorderAndShadow: yes";
"Collisions: Normal";
"Scroll Position: 1";
"Active Line: 7";
"Video Zoom Percent: 1";
"Last Style Storage: Default";
"Audio URI: Sukeban Deka II ed 3.avi";
"YCbCr Matrix: None";
"";
"[V4+ Styles]";
"Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding";
"Style: Default,Arial,20,&H00FFFFFF,&H000000FF,&H00000000,&H00000000,0,0,0,0,100,100,0,0,1,2,2,8,10,10,10,1";
"";
"[Events]";
"Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text"];;

let writefile f l = let a = open_out f in List.iter (fun x->output_string a (x^"\n")) l;close_out a;;

let writeAssToFile assStrings file =   writefile file (assHeader@assStrings);;
