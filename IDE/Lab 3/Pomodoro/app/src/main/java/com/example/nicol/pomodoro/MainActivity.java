package com.example.nicol.pomodoro;

import java.util.concurrent.TimeUnit;
import android.app.Activity;
import android.content.Context;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
    Button btnStart, btnStop,btnBreak;
    TextView textViewTime,tv;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnStart = (Button)findViewById(R.id.btnStart);
        btnStop = (Button)findViewById(R.id.btnStop);
        btnBreak = (Button)findViewById(R.id.btnBreak);
        textViewTime = (TextView)findViewById(R.id.textViewTime);
        tv = (TextView)findViewById(R.id.tv);
        textViewTime.setText("00:03:00");
        final PomodoroTimer timer = new PomodoroTimer(180000,1000);
        final PomodoroTimer timerbreak = new PomodoroTimer(60000,1000);
        btnStart.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                timer.start();
            }
        });
        btnStop.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                timer.cancel();
            }
        });
        btnBreak.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                timerbreak.start();
            }
        });
    }
    public void ringtone(){
        try {
            Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
            Ringtone r = RingtoneManager.getRingtone(getApplicationContext(), notification);
            r.play();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public class PomodoroTimer extends CountDownTimer {
        private Context context;

        public PomodoroTimer(long millisInFuture, long countDownInterval) {
            super(millisInFuture, countDownInterval);
        }
        @Override
        public void onFinish() {
            tv.setText("Break!!!");
            textViewTime.setText("00:01:00");
            ringtone();

        }
        @Override
        public void onTick(long millisUntilFinished) {
            long millis = millisUntilFinished;
            String hms = String.format("%02d:%02d:%02d", TimeUnit.MILLISECONDS.toHours(millis),
                    TimeUnit.MILLISECONDS.toMinutes(millis) - TimeUnit.HOURS.toMinutes(TimeUnit.MILLISECONDS.toHours(millis)),
                    TimeUnit.MILLISECONDS.toSeconds(millis) - TimeUnit.MINUTES.toSeconds(TimeUnit.MILLISECONDS.toMinutes(millis)));
            System.out.println(hms);
            textViewTime.setText(hms);
            tv.setText("");
        }
    }
}