# SunFlower
This is a set of geometric calculation objects and algorithms which can be used for computer aided design , analysis and manufacturing applications . It will be free and open as possible as it can be

Türk Geometrik Modelleme Kerneli Projesi namı diğer Ay Çekirdeği :)
(İsim SAHA İstanbul yöneticilerinden Ümit Karadeniz Bey tarafından telaffuz edilmiştir.) 

Temel ilkel geometri tipleri
Nokta ve vektör  (2 ve 3 boyutlu uzayda) 
Çizgi (2 ve 3 boyutlu uzayda)
Daire (2 ve 3 boyutlu uzayda)
Koordinat sistemi
Lineer cebir temelleri (Matrisler ve temel fonksiyonları)

Temel ilkel geometri kümeleri:
Polyline lar (3 boyutlu uzayda birbirine bağlı çizgiler)
Üçgen bulutu (3 boyutlu uzayda birbirine bağlı üçgenler)
Nokta bulutu (3 boyutlu uzayda noktalar)

Temel parametrik geometri tiplerinin tanımlanması:
 Eğriler çizgi daire elips konik arklar bezier , b-spline...,
 Yüzeyler uzatılmış eğri, raylı , döndürülmüş eğri ,  Bezier , NURBS... 
Sınırlandırılmış eğriler
Ofsetlenmiş eğriler 
Kesilmiş yüzeyler 
Ofsetlenmiş yüzeyler  

Bu tipler ile çalışan geometrik fonksiyonlar:
Sayısallaştırma (parametrik eğrilerden tolerans ile polyline üretme , parametrik yüzeylerden nokta bulutu veya tolerans ile üçgen bulutu üretme) 
Kesilmiş yüzeylerden tolerans ile üçgen bulutu üretme (Mesh generation)
Mesafe ölçme fonksiyonları(temel ilkel geometri kümeleri arasında  en kısa yol hesaplama: nokta-polyline , polyline-polyline,üçgen bulutu-nokta,üçgen bulutu-polyline...)
Kesişim fonksiyonları (temel ilkel geometri kümeleri arasındaki kesişimler , polyline-polyline,üçgen bulutu polyline , üçgen bulutu-üçgen bulutu)
Çevreleyen kutu hesaplama (axis aligned bounding box , minimum volume bounding box)
Tüm geometri tipleri  üzerinde lineer transformasyonlar
Parametrik uzay ile 3 boyutlu gerçek uzay arasındaki dönüşümler  (parametrik eğrilerde uzayda verilen  bir 3 boyutlu nokta için parametre uzayındaki t yi bulma , yüzeyde u,v parametrelerini bulma)
Çarpma kontrolü 
Minkowski Sum
Hull calculation
Bin packing 
True shape nesting

Topoloji
2 2.5 ve 3 boyutta katı model tanımları
Katılar üzerinde boolean operasyon fonksiyonları

Bu tipler ile çalışan temel fonksiyonlar :
Kayıt etme 
IGES – STEP – STL gibi kabul görmüş formatlardan içe alma ya da dışarı ihraç etme

Bu tipleri kullanarak çalışan grafik ekran boyama fonksiyonları:
Ekranda boyama 
Seçim

README.md
