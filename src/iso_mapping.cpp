#include "iso_mapping.h"


const QHash<QString, QString> COUNTRY_MAP = create_countries_map();
const QHash<QString, QString> LANG_MAP = create_lang_map();
const QStringList LANGUAGES = get_values(LANG_MAP);
const QStringList COUNTRIES = get_values(COUNTRY_MAP);

QStringList get_values(const QHash<QString, QString>& map) {
    QStringList values;
    for (QHash<QString, QString>::const_iterator it = map.constBegin(); it != map.constEnd(); ++it)
        values << it.value();
    return values;
}

QHash<QString, QString> create_countries_map()
{
    QHash<QString,QString> _ISO_MAP;
    _ISO_MAP["AF"] = "AFGHANISTAN";
    _ISO_MAP["AX"] = "ALAND ISLANDS";
    _ISO_MAP["AL"] = "ALBANIA";
    _ISO_MAP["DZ"] = "ALGERIA";
    _ISO_MAP["AS"] = "AMERICAN SAMOA";
    _ISO_MAP["AD"] = "ANDORRA";
    _ISO_MAP["AO"] = "ANGOLA";
    _ISO_MAP["AI"] = "ANGUILLA";
    _ISO_MAP["AQ"] = "ANTARCTICA";
    _ISO_MAP["AG"] = "ANTIGUA AND BARBUDA";
    _ISO_MAP["AR"] = "ARGENTINA";
    _ISO_MAP["AM"] = "ARMENIA";
    _ISO_MAP["AW"] = "ARUBA";
    _ISO_MAP["AU"] = "AUSTRALIA";
    _ISO_MAP["AT"] = "AUSTRIA";
    _ISO_MAP["AZ"] = "AZERBAIJAN";
    _ISO_MAP["BS"] = "BAHAMAS";
    _ISO_MAP["BH"] = "BAHRAIN";
    _ISO_MAP["BD"] = "BANGLADESH";
    _ISO_MAP["BB"] = "BARBADOS";
    _ISO_MAP["BY"] = "BELARUS";
    _ISO_MAP["BE"] = "BELGIUM";
    _ISO_MAP["BZ"] = "BELIZE";
    _ISO_MAP["BJ"] = "BENIN";
    _ISO_MAP["BM"] = "BERMUDA";
    _ISO_MAP["BT"] = "BHUTAN";
    _ISO_MAP["BO"] = "BOLIVIA";
    _ISO_MAP["BA"] = "BOSNIA AND HERZEGOVINA";
    _ISO_MAP["BW"] = "BOTSWANA";
    _ISO_MAP["BV"] = "BOUVET ISLAND";
    _ISO_MAP["BR"] = "BRAZIL";
    _ISO_MAP["VG"] = "BRITISH VIRGIN ISLANDS";
    _ISO_MAP["IO"] = "BRITISH INDIAN OCEAN TERRITORY";
    _ISO_MAP["BN"] = "BRUNEI DARUSSALAM";
    _ISO_MAP["BG"] = "BULGARIA";
    _ISO_MAP["BF"] = "BURKINA FASO";
    _ISO_MAP["BI"] = "BURUNDI";
    _ISO_MAP["KH"] = "CAMBODIA";
    _ISO_MAP["CM"] = "CAMEROON";
    _ISO_MAP["CA"] = "CANADA";
    _ISO_MAP["CV"] = "CAPE VERDE";
    _ISO_MAP["KY"] = "CAYMAN ISLANDS";
    _ISO_MAP["CF"] = "CENTRAL AFRICAN REPUBLIC";
    _ISO_MAP["TD"] = "CHAD";
    _ISO_MAP["CL"] = "CHILE";
    _ISO_MAP["CN"] = "CHINA";
    _ISO_MAP["HK"] = "HONG KONG, SPECIAL ADMINISTRATIVE REGION OF CHINA";
    _ISO_MAP["MO"] = "MACAO, SPECIAL ADMINISTRATIVE REGION OF CHINA";
    _ISO_MAP["CX"] = "CHRISTMAS ISLAND";
    _ISO_MAP["CC"] = "COCOS (KEELING) ISLANDS";
    _ISO_MAP["CO"] = "COLOMBIA";
    _ISO_MAP["KM"] = "COMOROS";
    _ISO_MAP["CG"] = "CONGO (BRAZZAVILLE)";
    _ISO_MAP["CD"] = "CONGO, DEMOCRATIC REPUBLIC OF THE";
    _ISO_MAP["CK"] = "COOK ISLANDS";
    _ISO_MAP["CR"] = "COSTA RICA";
    _ISO_MAP["CI"] = "CÔTE D'IVOIRE";
    _ISO_MAP["HR"] = "CROATIA";
    _ISO_MAP["CU"] = "CUBA";
    _ISO_MAP["CY"] = "CYPRUS";
    _ISO_MAP["CZ"] = "CZECH REPUBLIC";
    _ISO_MAP["DK"] = "DENMARK";
    _ISO_MAP["DJ"] = "DJIBOUTi";
    _ISO_MAP["DM"] = "DOMINICa";
    _ISO_MAP["DO"] = "DOMINICAN REPUBLIC";
    _ISO_MAP["EC"] = "ECUADOR";
    _ISO_MAP["EG"] = "EGYPT";
    _ISO_MAP["SV"] = "EL SALVADOR";
    _ISO_MAP["GQ"] = "EQUATORIAL GUINEA";
    _ISO_MAP["ER"] = "ERITREA";
    _ISO_MAP["EE"] = "ESTONIA";
    _ISO_MAP["ET"] = "ETHIOPIa";
    _ISO_MAP["FK"] = "FALKLAND ISLANDS (MALVINAS)";
    _ISO_MAP["FO"] = "FAROE ISLANDS";
    _ISO_MAP["FJ"] = "FIJI";
    _ISO_MAP["FI"] = "FINLAND";
    _ISO_MAP["FR"] = "FRANCE";
    _ISO_MAP["GF"] = "FRENCH GUIANA";
    _ISO_MAP["PF"] = "FRENCH POLYNESIA";
    _ISO_MAP["TF"] = "FRENCH SOUTHERN TERRITORIES";
    _ISO_MAP["GA"] = "GABON";
    _ISO_MAP["GM"] = "GAMBIA";
    _ISO_MAP["GE"] = "GEORGIA";
    _ISO_MAP["DE"] = "GERMANY";
    _ISO_MAP["GH"] = "GHANA";
    _ISO_MAP["GI"] = "GIBRALTAR";
    _ISO_MAP["GR"] = "GREECE";
    _ISO_MAP["GL"] = "GREENLAND";
    _ISO_MAP["GD"] = "GRENADA";
    _ISO_MAP["GP"] = "GUADELOUPE";
    _ISO_MAP["GU"] = "GUAM";
    _ISO_MAP["GT"] = "GUATEMALA";
    _ISO_MAP["GG"] = "GUERNSEY";
    _ISO_MAP["GN"] = "GUINEA";
    _ISO_MAP["GW"] = "GUINEA-BISSAU";
    _ISO_MAP["GY"] = "GUYANA";
    _ISO_MAP["HT"] = "HAITI";
    _ISO_MAP["HM"] = "HEARD ISLAND AND MCDONALD ISLANDS";
    _ISO_MAP["VA"] = "HOLY SEE (VATICAN CITY STATE)";
    _ISO_MAP["HN"] = "HONDURAS";
    _ISO_MAP["HU"] = "HUNGARY";
    _ISO_MAP["IS"] = "ICELAND";
    _ISO_MAP["IN"] = "INDIA";
    _ISO_MAP["ID"] = "INDONESIA";
    _ISO_MAP["IR"] = "IRAN, ISLAMIC REPUBLIC OF";
    _ISO_MAP["IQ"] = "IRAQ";
    _ISO_MAP["IE"] = "IRELAND";
    _ISO_MAP["IM"] = "ISLE OF MAN";
    _ISO_MAP["IL"] = "ISRAEL";
    _ISO_MAP["IT"] = "ITALY";
    _ISO_MAP["JM"] = "JAMAICA";
    _ISO_MAP["JP"] = "JAPAN";
    _ISO_MAP["JE"] = "JERSEY";
    _ISO_MAP["JO"] = "JORDAN";
    _ISO_MAP["KZ"] = "KAZAKHSTAN";
    _ISO_MAP["KE"] = "KENYA";
    _ISO_MAP["KI"] = "KIRIBATI";
    _ISO_MAP["KP"] = "KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF";
    _ISO_MAP["KR"] = "KOREA, REPUBLIC OF";
    _ISO_MAP["KW"] = "KUWAIT";
    _ISO_MAP["KG"] = "KYRGYZSTAN";
    _ISO_MAP["LA"] = "LAO PDR";
    _ISO_MAP["LV"] = "LATVIA";
    _ISO_MAP["LB"] = "LEBANON";
    _ISO_MAP["LS"] = "LESOTHO";
    _ISO_MAP["LR"] = "LIBERIA";
    _ISO_MAP["LY"] = "LIBYA";
    _ISO_MAP["LI"] = "LIECHTENSTEIN";
    _ISO_MAP["LT"] = "LITHUANIA";
    _ISO_MAP["LU"] = "LUXEMBOURG";
    _ISO_MAP["MK"] = "MACEDONIA, REPUBLIC OF";
    _ISO_MAP["MG"] = "MADAGASCAR";
    _ISO_MAP["MW"] = "MALAWI";
    _ISO_MAP["MY"] = "MALAYSIA";
    _ISO_MAP["MV"] = "MALDIVES";
    _ISO_MAP["ML"] = "MALI";
    _ISO_MAP["MT"] = "MALTA";
    _ISO_MAP["MH"] = "MARSHALL ISLANDS";
    _ISO_MAP["MQ"] = "MARTINIQUE";
    _ISO_MAP["MR"] = "MAURITANIA";
    _ISO_MAP["MU"] = "MAURITIUS";
    _ISO_MAP["YT"] = "MAYOTTE";
    _ISO_MAP["MX"] = "MEXICO";
    _ISO_MAP["FM"] = "MICRONESIA, FEDERATED STATES OF";
    _ISO_MAP["MD"] = "MOLDOVA";
    _ISO_MAP["MC"] = "MONACO";
    _ISO_MAP["MN"] = "MONGOLIA";
    _ISO_MAP["ME"] = "MONTENEGRO";
    _ISO_MAP["MS"] = "MONTSERRAT";
    _ISO_MAP["MA"] = "MOROCCO";
    _ISO_MAP["MZ"] = "MOZAMBIQUE";
    _ISO_MAP["MM"] = "MYANMAR";
    _ISO_MAP["NA"] = "NAMIBIA";
    _ISO_MAP["NR"] = "NAURU";
    _ISO_MAP["NP"] = "NEPAL";
    _ISO_MAP["NL"] = "NETHERLANDS";
    _ISO_MAP["AN"] = "NETHERLANDS ANTILLES";
    _ISO_MAP["NC"] = "NEW CALEDONIA";
    _ISO_MAP["NZ"] = "NEW ZEALAND";
    _ISO_MAP["NI"] = "NICARAGUA";
    _ISO_MAP["NE"] = "NIGER";
    _ISO_MAP["NG"] = "NIGERIA";
    _ISO_MAP["NU"] = "NIUE";
    _ISO_MAP["NF"] = "NORFOLK ISLAND";
    _ISO_MAP["MP"] = "NORTHERN MARIANA ISLANDS";
    _ISO_MAP["NO"] = "NORWAY";
    _ISO_MAP["OM"] = "OMAN";
    _ISO_MAP["PK"] = "PAKISTAN";
    _ISO_MAP["PW"] = "PALAU";
    _ISO_MAP["PS"] = "PALESTINIAN TERRITORY, OCCUPIED";
    _ISO_MAP["PA"] = "PANAMA";
    _ISO_MAP["PG"] = "PAPUA NEW GUINEA";
    _ISO_MAP["PY"] = "PARAGUAY";
    _ISO_MAP["PE"] = "PERU";
    _ISO_MAP["PH"] = "PHILIPPINES";
    _ISO_MAP["PN"] = "PITCAIRN";
    _ISO_MAP["PL"] = "POLAND";
    _ISO_MAP["PT"] = "PORTUGAL";
    _ISO_MAP["PR"] = "PUERTO RICO";
    _ISO_MAP["QA"] = "QATAR";
    _ISO_MAP["RE"] = "RÉUNION";
    _ISO_MAP["RO"] = "ROMANIA";
    _ISO_MAP["RU"] = "RUSSIAN FEDERATION";
    _ISO_MAP["RW"] = "RWANDA";
    _ISO_MAP["BL"] = "SAINT-BARTHÉLEMY";
    _ISO_MAP["SH"] = "SAINT HELENA";
    _ISO_MAP["KN"] = "SAINT KITTS AND NEVIS";
    _ISO_MAP["LC"] = "SAINT LUCIA";
    _ISO_MAP["MF"] = "SAINT-MARTIN (FRENCH PART)";
    _ISO_MAP["PM"] = "SAINT PIERRE AND MIQUELON";
    _ISO_MAP["VC"] = "SAINT VINCENT AND GRENADINES";
    _ISO_MAP["WS"] = "SAMOA";
    _ISO_MAP["SM"] = "SAN MARINO";
    _ISO_MAP["ST"] = "SAO TOME AND PRINCIPE";
    _ISO_MAP["SA"] = "SAUDI ARABIA";
    _ISO_MAP["SN"] = "SENEGAL";
    _ISO_MAP["RS"] = "SERBIA";
    _ISO_MAP["SC"] = "SEYCHELLES";
    _ISO_MAP["SL"] = "SIERRA LEONE";
    _ISO_MAP["SG"] = "SINGAPORE";
    _ISO_MAP["SK"] = "SLOVAKIA";
    _ISO_MAP["SI"] = "SLOVENIA";
    _ISO_MAP["SB"] = "SOLOMON ISLANDS";
    _ISO_MAP["SO"] = "SOMALIA";
    _ISO_MAP["ZA"] = "SOUTH AFRICA";
    _ISO_MAP["GS"] = "SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS";
    _ISO_MAP["SS"] = "SOUTH SUDAN";
    _ISO_MAP["ES"] = "SPAIN";
    _ISO_MAP["LK"] = "SRI LANKA";
    _ISO_MAP["SD"] = "SUDAN";
    _ISO_MAP["SR"] = "SURINAME";
    _ISO_MAP["SJ"] = "SVALBARD AND JAN MAYEN ISLANDS";
    _ISO_MAP["SZ"] = "SWAZILAND";
    _ISO_MAP["SE"] = "SWEDEN";
    _ISO_MAP["CH"] = "SWITZERLAND";
    _ISO_MAP["SY"] = "SYRIAN ARAB REPUBLIC (SYRIA)";
    _ISO_MAP["TW"] = "TAIWAN, REPUBLIC OF CHINA";
    _ISO_MAP["TJ"] = "TAJIKISTAN";
    _ISO_MAP["TZ"] = "TANZANIA";
    _ISO_MAP["TH"] = "THAILAND";
    _ISO_MAP["TL"] = "TIMOR-LESTE";
    _ISO_MAP["TG"] = "TOGO";
    _ISO_MAP["TK"] = "TOKELAU";
    _ISO_MAP["TO"] = "TONGA";
    _ISO_MAP["TT"] = "TRINIDAD AND TOBAGO";
    _ISO_MAP["TN"] = "TUNISIA";
    _ISO_MAP["TR"] = "TURKEY";
    _ISO_MAP["TM"] = "TURKMENISTAN";
    _ISO_MAP["TC"] = "TURKS AND CAICOS ISLANDS";
    _ISO_MAP["TV"] = "TUVALU";
    _ISO_MAP["UG"] = "UGANDA";
    _ISO_MAP["UA"] = "UKRAINE";
    _ISO_MAP["AE"] = "UNITED ARAB EMIRATES";
    _ISO_MAP["GB"] = "UNITED KINGDOM";
    _ISO_MAP["US"] = "UNITED STATES OF AMERICA";
    _ISO_MAP["UM"] = "UNITED STATES MINOR OUTLYING ISLANDS";
    _ISO_MAP["UY"] = "URUGUAY";
    _ISO_MAP["UZ"] = "UZBEKISTAN";
    _ISO_MAP["VU"] = "VANUATU";
    _ISO_MAP["VE"] = "VENEZUELA (BOLIVARIAN REPUBLIC OF)";
    _ISO_MAP["VN"] = "VIET NAM";
    _ISO_MAP["VI"] = "VIRGIN ISLANDS";
    _ISO_MAP["WF"] = "WALLIS AND FUTUNA ISLANDS";
    _ISO_MAP["EH"] = "WESTERN SAHARA";
    _ISO_MAP["YE"] = "YEMEN";
    _ISO_MAP["ZM"] = "ZAMBIA";
    _ISO_MAP["ZW"] = "ZIMBABWE";
    return _ISO_MAP;
}

QHash<QString, QString> create_lang_map()
{
    QHash<QString,QString> _LANG_MAP;
    _LANG_MAP["aa"] = "AFAR";
    _LANG_MAP["ab"] = "ABKHAZIAN";
    _LANG_MAP["ae"] = "AVESTAN";
    _LANG_MAP["af"] = "AFRIKAANS";
    _LANG_MAP["ak"] = "AKAN";
    _LANG_MAP["am"] = "AMHARIC";
    _LANG_MAP["an"] = "ARAGONESE";
    _LANG_MAP["ar"] = "ARABIC";
    _LANG_MAP["as"] = "ASSAMESE";
    _LANG_MAP["av"] = "AVARIC";
    _LANG_MAP["ay"] = "AYMARA";
    _LANG_MAP["az"] = "AZERBAIJANI";
    _LANG_MAP["ba"] = "BASHKIR";
    _LANG_MAP["be"] = "BELARUSIAN";
    _LANG_MAP["bg"] = "BULGARIAN";
    _LANG_MAP["bh"] = "BIHARI";
    _LANG_MAP["bi"] = "BISLAMA";
    _LANG_MAP["bm"] = "BAMBARA";
    _LANG_MAP["bn"] = "BENGALI";
    _LANG_MAP["bo"] = "TIBETAN";
    _LANG_MAP["br"] = "BRETON";
    _LANG_MAP["bs"] = "BOSNIAN";
    _LANG_MAP["my"] = "BURMESE";
    _LANG_MAP["ca"] = "CATALAN";
    _LANG_MAP["ce"] = "CHECHEN";
    _LANG_MAP["ch"] = "CHAMORRO";
    _LANG_MAP["co"] = "CORSICAN";
    _LANG_MAP["cr"] = "CREE";
    _LANG_MAP["cs"] = "CZECH";
    _LANG_MAP["cu"] = "CHURCH SLAVIC";
    _LANG_MAP["cv"] = "CHUVASH";
    _LANG_MAP["cy"] = "WELSH";
    _LANG_MAP["da"] = "DANISH";
    _LANG_MAP["de"] = "GERMAN";
    _LANG_MAP["dv"] = "DIVEHI";
    _LANG_MAP["dz"] = "DZONGKHA";
    _LANG_MAP["ee"] = "EWE";
    _LANG_MAP["el"] = "GREEK";
    _LANG_MAP["en"] = "ENGLISH";
    _LANG_MAP["eo"] = "ESPERANTO";
    _LANG_MAP["es"] = "SPANISH";
    _LANG_MAP["et"] = "ESTONIAN";
    _LANG_MAP["eu"] = "BASQUE";
    _LANG_MAP["fa"] = "PERSIAN";
    _LANG_MAP["ff"] = "FULAH";
    _LANG_MAP["fi"] = "FINNISH";
    _LANG_MAP["fj"] = "FIJIAN";
    _LANG_MAP["fo"] = "FAROESE";
    _LANG_MAP["fr"] = "FRENCH";
    _LANG_MAP["fy"] = "WESTERN FRISON";
    _LANG_MAP["ga"] = "IRISH";
    _LANG_MAP["gd"] = "GAELIC";
    _LANG_MAP["gl"] = "GALICIAN";
    _LANG_MAP["gn"] = "GUARANI";
    _LANG_MAP["gu"] = "GUJARATI";
    _LANG_MAP["gv"] = "MANX";
    _LANG_MAP["ha"] = "HAUSA";
    _LANG_MAP["he"] = "HEBREW";
    _LANG_MAP["hi"] = "HINDI";
    _LANG_MAP["ho"] = "HIRI MOTU";
    _LANG_MAP["hr"] = "CROATIAN";
    _LANG_MAP["ht"] = "HAITIAN";
    _LANG_MAP["hu"] = "HUNGARIAN";
    _LANG_MAP["hy"] = "ARMENIAN";
    _LANG_MAP["hz"] = "HERERO";
    _LANG_MAP["ia"] = "INTERLINGUA";
    _LANG_MAP["id"] = "INDONESIAN";
    _LANG_MAP["ie"] = "INTERLINGUE";
    _LANG_MAP["ig"] = "IGBO";
    _LANG_MAP["ii"] = "SICHUAN YI";
    _LANG_MAP["ik"] = "INUPIAQ";
    _LANG_MAP["id"] = "INDONESIAN";
    _LANG_MAP["io"] = "IDO";
    _LANG_MAP["is"] = "ICELANDIC";
    _LANG_MAP["it"] = "ITALIAN";
    _LANG_MAP["he"] = "HEBREW";
    _LANG_MAP["iu"] = "INUKTITUT";
    _LANG_MAP["ja"] = "JAPANESE";
    _LANG_MAP["yi"] = "YIDDISH";
    _LANG_MAP["jv"] = "JAVANESE";
    _LANG_MAP["jv"] = "JAVANESE";
    _LANG_MAP["ka"] = "GEORGIAN";
    _LANG_MAP["kg"] = "KONGO";
    _LANG_MAP["ki"] = "KIKUYU";
    _LANG_MAP["kj"] = "KWANYAMA";
    _LANG_MAP["kk"] = "KAZAKH";
    _LANG_MAP["kl"] = "KALAALLISUT";
    _LANG_MAP["km"]	= "CENTRAL KHMER";
    _LANG_MAP["kn"] = "KANNADA";
    _LANG_MAP["ko"] = "KOREAN";
    _LANG_MAP["kr"] = "KANURI";
    _LANG_MAP["ks"] = "KASHMIRI";
    _LANG_MAP["ku"] = "KURDISH";
    _LANG_MAP["kv"] = "KOMI";
    _LANG_MAP["kw"] = "CORNISH";
    _LANG_MAP["ky"] = "KIRGHIZ";
    _LANG_MAP["la"] = "LATIN";
    _LANG_MAP["lb"] = "LUXEMBOURGISH";
    _LANG_MAP["lg"] = "GANDA";
    _LANG_MAP["li"] = "LIMBURGISH";
    _LANG_MAP["ln"] = "LINGALA";
    _LANG_MAP["lo"] = "LAO";
    _LANG_MAP["lt"] = "LITHUANIAN";
    _LANG_MAP["lu"] = "LUBA-KATANGA";
    _LANG_MAP["lv"] = "LATVIAN";
    _LANG_MAP["mg"] = "MALAGASY";
    _LANG_MAP["mh"] = "MARSHALLESE";
    _LANG_MAP["mi"] = "MAORI";
    _LANG_MAP["mk"] = "MACEDONIAN";
    _LANG_MAP["ml"] = "MALAYALAM";
    _LANG_MAP["mn"] = "MONGOLIAN";
    _LANG_MAP["ro"] = "MOLDAVIAN ;";
    _LANG_MAP["mr"] = "MARATHI";
    _LANG_MAP["ms"] = "MALAY";
    _LANG_MAP["mt"] = "MALTESE";
    _LANG_MAP["my"] = "BURMESE";
    _LANG_MAP["na"] = "NAURU";
    _LANG_MAP["nb"] = "NORWEGIAN";
    _LANG_MAP["nd"] = "NORTH NDEBELE";
    _LANG_MAP["ne"] = "NEPALESE";
    _LANG_MAP["ng"] = "NDONGA";
    _LANG_MAP["nl"] = "FLEMISH";
    _LANG_MAP["nn"] = "NORWEGIAN";
    _LANG_MAP["no"] = "NORWEGIAN";
    _LANG_MAP["nr"] = "SOUTH NDEDELE";
    _LANG_MAP["nv"] = "NAVAHO";
    _LANG_MAP["ny"] = "NYANJA";
    _LANG_MAP["oc"] = "OCCITAN";
    _LANG_MAP["oj"] = "OJIBWA";
    _LANG_MAP["om"] = "OROMO";
    _LANG_MAP["or"] = "ORIYA";
    _LANG_MAP["os"] = "OSSETIAN";
    _LANG_MAP["pa"] = "PANJABI";
    _LANG_MAP["pi"] = "PALI";
    _LANG_MAP["pl"] = "POLISH";
    _LANG_MAP["ps"] = "PASHTO";
    _LANG_MAP["pt"] = "PORTUGUESE";
    _LANG_MAP["qu"] = "QUECHUA";
    _LANG_MAP["rm"] = "ROMANSH";
    _LANG_MAP["rn"] = "RUNDI";
    _LANG_MAP["ro"] = "MOLDOVAN";
    _LANG_MAP["ru"] = "RUSSIAN";
    _LANG_MAP["rw"] = "KINYARWANDA";
    _LANG_MAP["sa"] = "SANSKRIT";
    _LANG_MAP["sc"] = "SARDINIAN";
    _LANG_MAP["sd"] = "SINDHI";
    _LANG_MAP["se"] = "NORTHERN SAMI";
    _LANG_MAP["sg"] = "SANGO";
    _LANG_MAP["bs"] = "SERBO-CROATIAN";
    _LANG_MAP["si"] = "SINHALESE";
    _LANG_MAP["sk"] = "SLOVAK";
    _LANG_MAP["sl"] = "SLOVENIAN";
    _LANG_MAP["sm"] = "SAMOAN";
    _LANG_MAP["sn"] = "SHONA";
    _LANG_MAP["so"] = "SOMALI";
    _LANG_MAP["sq"] = "ALBANAIS";
    _LANG_MAP["sr"] = "SERBIAN";
    _LANG_MAP["ss"] = "SWATI";
    _LANG_MAP["st"] = "SOTHO, SOUTHERN";
    _LANG_MAP["su"] = "SUNDANESE";
    _LANG_MAP["sv"] = "SWEDISH";
    _LANG_MAP["sw"] = "SWAHILI";
    _LANG_MAP["ta"] = "TAMIL";
    _LANG_MAP["te"] = "TELUGU";
    _LANG_MAP["tg"] = "TAJIK";
    _LANG_MAP["th"] = "THAI";
    _LANG_MAP["ti"] = "TIGRINYA";
    _LANG_MAP["tk"] = "TURKMEN";
    _LANG_MAP["tl"] = "TAGALOG";
    _LANG_MAP["tn"] = "TSWANA";
    _LANG_MAP["to"] = "TONGA";
    _LANG_MAP["tr"] = "TURKISH";
    _LANG_MAP["ts"] = "TSONGA";
    _LANG_MAP["tt"] = "TATARE";
    _LANG_MAP["tw"] = "TWI";
    _LANG_MAP["ty"] = "TAHITIAN";
    _LANG_MAP["ug"] = "UYGHUR";
    _LANG_MAP["uk"] = "UKRAINIAN";
    _LANG_MAP["ur"] = "URDU";
    _LANG_MAP["uz"] = "UZBEK";
    _LANG_MAP["ve"] = "VENDA";
    _LANG_MAP["vi"] = "VIETNAMESE";
    _LANG_MAP["vo"] = "VOLAPÜK";
    _LANG_MAP["wa"] = "WALLOON";
    _LANG_MAP["wo"] = "WOLOF";
    _LANG_MAP["xh"] = "XHOSA";
    _LANG_MAP["yi"] = "YIDDISH";
    _LANG_MAP["yo"] = "YORUBA";
    _LANG_MAP["za"] = "CHUANG";
    _LANG_MAP["zh"] = "CHINESE";
    _LANG_MAP["zu"] = "ZULU";

    return _LANG_MAP;
}