package com.ns.earthquakealert

import android.app.AlarmManager
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import android.widget.Toast
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase
import com.google.firebase.messaging.FirebaseMessaging
import com.google.gson.Gson
import com.ns.earthquakealert.databinding.ActivityMainBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

const val TOPIC = "/topics/myTopic"
const val TAG = "MainActivity"

class MainActivity : AppCompatActivity() {

    private var database: DatabaseReference = Firebase.database.reference
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        FirebaseMessaging.getInstance().subscribeToTopic(TOPIC)


        binding.button.setOnClickListener {

            database.child("deprem").setValue(true)
            database.child("deprem").setValue(false)
        }

        getData()
    }

    private fun sendNotification(notification: PushNotification) =
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val response = RetrofitInstance.api.postNotification(notification)
                if (response.isSuccessful) {
                    Log.d(TAG, "Response: $response")
                } else {
                    Log.e(TAG, response.errorBody().toString())
                }
            } catch (e: Exception) {
                Log.e("MainActivity", e.toString())
            }
        }

    private fun getData() {
        database.child("deprem").addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val depremValue = snapshot.getValue(Boolean::class.java)
                if (depremValue == true) {
                    PushNotification(
                        NotificationData("Earthquake Alert!!", "DEPREM"),
                        TOPIC
                    ).also {
                        sendNotification(it)
                    }
                    Log.d(TAG, "DEPREM")
                } else {
                    Log.d(TAG, "Deprem değeri false")
                }
            }

            override fun onCancelled(error: DatabaseError) {
                Log.d(TAG,error.message)
            }
        })
    }

    override fun onStop() {
        super.onStop()
        startService(Intent(this, FirebaseService::class.java))
    }

    override fun onDestroy() {
        super.onDestroy()
        startService(Intent(this, FirebaseService::class.java))
    }
}