# hw2

# main function:
一開始先將初始的頻率選單畫面輸出在uLCD上，\
***然後使用無限迴圈不斷重複以下步驟***\
透過上下按鈕來選擇頻率，當下要被選到的頻率會呈現綠色文字背景，未被選到的頻率則為白色文字背景，\
當按下select按鈕後，被選定的頻率會顯示紅色背景，表示目前產生的波為這個頻率。\
(頻率共有3Hz,5Hz,10Hz,20Hz,37Hz,50Hz,60Hz,70Hz,85Hz,100Hz十種選擇，其中cut off 頻率為 f = 1/(2*pi*R*C) = 1/(2*pi*30500*3*47*10^-9) = 36.985 = 37Hz)\
\
接著呼叫wave generator函式，\
先計算在前0.4週期每100微秒波要上升多少Volt，但由於硬體設備造成延遲，\
因此將wait_us(100)改為wait_us(70)來修正誤差，每70微秒加上先前計算的delta Volt，\
同理，計算後0.6週期每100微秒波要下降多少Volt，再以每70微秒減去計算好的delta Volt，\
得到一週期的波形，並無限反覆輸出波形。\
\
最後使用thread加上eventqueue來呼叫ADC_sampling這個函式來對波型做sampling並將資料存取到陣列裡，\
在ADC_sampling這個函式裡的最後同樣使用thread加上eventqueue來呼叫screen_print這個函式\
來將存取好的資料輸出給screen，在screen_print函式的最後也呼叫ADC_sampling函式，\
使得sample跟print的函式不斷互相呼叫，進而達成sample與print時間互相錯開的效果，\
這麼做是因為print所需要的時間較長，因此先在不影響波形的狀況下對波進行sample，\
存完後再讓資料一次輸出，並且不採樣這時扭曲的波形，以確保sample的品質。\
除此之外，由於使用了thread的方法來存取sample到的資料以及輸出資料到screen上，因此也比較不會造成時間上的延遲。\
\
\
使用FFT python code來對sample好的資料作圖，結果如下:\
\
3Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017245-d6910800-91b1-11eb-835e-6c52fd6d1fad.png)

5Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017082-b103fe80-91b1-11eb-89b1-25f26272243d.png)

10Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017324-ead50500-91b1-11eb-90d5-e5827a276c29.png)

20Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017388-f7595d80-91b1-11eb-8642-4ea700864056.png)

37Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017514-1a840d00-91b2-11eb-9f3f-8ec7e03650b1.png)

50Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017578-2d96dd00-91b2-11eb-9541-565a2faea984.png)

60Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017704-55864080-91b2-11eb-8096-c766cc0e2c59.png)

70Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017755-659e2000-91b2-11eb-884e-47b16b72b486.png)

85Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017891-86667580-91b2-11eb-96cb-57d7436be797.png)

100Hz:\
![image](https://user-images.githubusercontent.com/79581724/113017937-954d2800-91b2-11eb-8cb7-40726e512309.png)

