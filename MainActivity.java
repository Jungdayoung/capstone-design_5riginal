package com.example.user.weather;

import android.content.ContentValues;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    public static final int THREAD_HANDLER_SUCCESS_INFO = 1;
    TextView tv_WeatherInfo;

    ForeCastManager mForeCast;

    String lon = "126.9093"; // 좌표 설정
    String lat = "35.165";  // 좌표 설정

    MainActivity mThis;
    ArrayList<ContentValues> mWeatherData;
    ArrayList<WeatherInfo> mWeatherInfomation;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Initialize();
    }

    public void Initialize()
    {
        tv_WeatherInfo = (TextView)findViewById(R.id.tv_WeatherInfo);
        mWeatherInfomation = new ArrayList<>();
        mThis = this;
        mForeCast = new ForeCastManager(lon,lat,mThis);
        mForeCast.run();
    }

    public String PrintValue()
    {
        String mData = "";
        for(int i = 0; i < mWeatherInfomation.size(); i ++)
        {
            mData = mData + mWeatherInfomation.get(i).getWeather_Day() + "\r\n"
                    +  mWeatherInfomation.get(i).getWeather_Name() + "\r\n"
                    +  mWeatherInfomation.get(i).getClouds_Sort()
                    +  " /Cloud amount: " + mWeatherInfomation.get(i).getClouds_Value()
                    +  mWeatherInfomation.get(i).getClouds_Per() +"\r\n"
                    +  mWeatherInfomation.get(i).getWind_Name()
                    +  " /WindSpeed: " + mWeatherInfomation.get(i).getWind_Speed() + " mps" + "\r\n"
                    +  "Max: " + mWeatherInfomation.get(i).getTemp_Max() + "℃"
                    +  " /Min: " + mWeatherInfomation.get(i).getTemp_Min() + "℃" +"\r\n"
                    +  "Humidity: " + mWeatherInfomation.get(i).getHumidity() + "%";

            mData = mData + "\r\n" + "----------------------------------------------" + "\r\n";
        }
        return mData;
    }

    public void DataChangedToHangeul()
    {
        for(int i = 0 ; i <mWeatherInfomation.size(); i ++)
        {
            WeatherToHangeul mHangeul = new WeatherToHangeul(mWeatherInfomation.get(i));
            mWeatherInfomation.set(i,mHangeul.getHangeulWeather());
        }
    }


    public void DataToInformation()
    {
        for(int i = 0; i < mWeatherData.size(); i++)
        {
            mWeatherInfomation.add(new WeatherInfo(
                    String.valueOf(mWeatherData.get(i).get("weather_Name")),  String.valueOf(mWeatherData.get(i).get("weather_Number")), String.valueOf(mWeatherData.get(i).get("weather_Much")),
                    String.valueOf(mWeatherData.get(i).get("weather_Type")),  String.valueOf(mWeatherData.get(i).get("wind_Direction")),  String.valueOf(mWeatherData.get(i).get("wind_SortNumber")),
                    String.valueOf(mWeatherData.get(i).get("wind_SortCode")),  String.valueOf(mWeatherData.get(i).get("wind_Speed")),  String.valueOf(mWeatherData.get(i).get("wind_Name")),
                    String.valueOf(mWeatherData.get(i).get("temp_Min")),  String.valueOf(mWeatherData.get(i).get("temp_Max")),  String.valueOf(mWeatherData.get(i).get("humidity")),
                    String.valueOf(mWeatherData.get(i).get("Clouds_Value")),  String.valueOf(mWeatherData.get(i).get("Clouds_Sort")), String.valueOf(mWeatherData.get(i).get("Clouds_Per")),String.valueOf(mWeatherData.get(i).get("day"))
            ));

        }

    }
    // 메인 UI스레드를 사용하기 위해, Message통신을 통해
    // 일종의 동기화 작업을 하기 위한 코드
    public Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch(msg.what){
                case THREAD_HANDLER_SUCCESS_INFO :
                    mForeCast.getmWeather();
                    mWeatherData = mForeCast.getmWeather();
                    if(mWeatherData.size() ==0)
                        tv_WeatherInfo.setText("데이터가 없습니다");

                    DataToInformation(); // 자료 클래스로 저장,
                    String data = "";

                    data = PrintValue();
                    DataChangedToHangeul();
                    data = data + PrintValue();

                    tv_WeatherInfo.setText(data);
                    break;
                default:
                    break;
            }
        }


    };
}
