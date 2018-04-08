package com.example.user.weather;

import android.content.ContentValues;

/**
 * Created by WooChan on 2016-01-18.
 */
public class WeatherToHangeul {
    WeatherConditionList mCondition;
    ContentValues mData;
    WeatherInfo mWeatherInfo;

    public WeatherToHangeul(WeatherInfo tData)
    {
        mCondition = new WeatherConditionList();
        mWeatherInfo = tData;


        mWeatherInfo.setClouds_Sort(Hangeul_Weather(mWeatherInfo.clouds_Sort));
        mWeatherInfo.setWeather_Name(Hangeul_Weather(mWeatherInfo.weather_Number));
        mWeatherInfo.setWind_Name(Hangeul_Weather(mWeatherInfo.wind_Name));
    }

    public WeatherInfo getHangeulWeather()
    {
        return mWeatherInfo;
    }

    public String SnowToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListSnow.size() ; i++)
        {
            if(mCondition.mListSnow.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListSnow.get(i).getMeaning().equals(weatherNumber))
                return mCondition.mListSnowToHangeul.get(i).getMeaning();
        }

        return "";
    }
    public String ClearToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListClearSky.size() ; i++)
        {
            if(mCondition.mListClearSky.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListClearSky.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListClearSkyToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String BrokenCloudsToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListBroken_Clouds.size() ; i++)
        {
            if(mCondition.mListBroken_Clouds.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListBroken_Clouds.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListBroken_CloudsToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String FewCloudsToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListFew_Clouds.size() ; i++)
        {
            if(mCondition.mListFew_Clouds.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListFew_Clouds.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListFew_CloudsToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String ScatteredCloudsToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListScattered_Clouds.size() ; i++)
        {
            if(mCondition.mListScattered_Clouds.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListScattered_Clouds.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListScattered_CloudsToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String RainToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListRain.size() ; i++)
        {
            if(mCondition.mListRain.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListRain.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListRainToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String ShowerRainToHanGeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListShower_Rain.size() ; i++)
        {
            if(mCondition.mListShower_Rain.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListShower_Rain.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListShower_RainToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String ThunderStromToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListThunderStorm.size() ; i++)
        {
            if(mCondition.mListThunderStorm.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListThunderStorm.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListThunderStormToHangeul.get(i).getMeaning();
        }
        return "";
    }
    public String MistToHangeul(String weatherNumber)
    {
        for(int i = 0; i < mCondition.mListMist.size() ; i++)
        {
            if(mCondition.mListMist.get(i).getId().equals(weatherNumber) ||
                    mCondition.mListMist.get(i).getMeaning().equals(weatherNumber.toLowerCase()))
                return mCondition.mListMistToHangeul.get(i).getMeaning();
        }
        return "";
    }

    public String WindToHangeul(String windName)
    {
        for(int i = 0; i < mCondition.mListWind.size() ; i++) {
            if(mCondition.mListWind.get(i).getId().equals(windName)
                    || mCondition.mListWind.get(i).getMeaning().equals(windName.toLowerCase()))
                return mCondition.mListWindToHangeul.get(i).getMeaning();
        }
        return "";
    }

    public String Hangeul_Weather(String mWeatherNumber)
    {
        String snow = SnowToHangeul(mWeatherNumber);
        String clear = ClearToHangeul(mWeatherNumber);
        String broken_Cloud = BrokenCloudsToHangeul(mWeatherNumber);
        String few_Cloud = FewCloudsToHangeul(mWeatherNumber);
        String scatter = ScatteredCloudsToHangeul(mWeatherNumber);
        String Rain = RainToHangeul(mWeatherNumber);
        String shower = ShowerRainToHanGeul(mWeatherNumber);
        String thunder = ThunderStromToHangeul(mWeatherNumber);
        String mist = MistToHangeul(mWeatherNumber);
        String wind = WindToHangeul(mWeatherNumber);


        if(!snow.equals("")) return snow;
        else if(!clear.equals("")) return clear;
        else if(!broken_Cloud.equals("")) return broken_Cloud;
        else if(!few_Cloud.equals("")) return few_Cloud;
        else if(!scatter.equals("")) return scatter;
        else if(!Rain.equals("")) return Rain;
        else if(!shower.equals("")) return shower;
        else if(!thunder.equals("")) return thunder;
        else if(!mist.equals("")) return mist;
        else if(!wind.equals("")) return wind;

        return "정보없음";
    }
}
