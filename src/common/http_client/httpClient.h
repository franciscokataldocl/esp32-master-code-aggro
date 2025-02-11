// #include <HTTPClient.h> 
// enum HttpMethod {
//     GET,
//     POST,
//     PUT,
//     DELETE,
//     PATCH
// };

// void sendHttpRequest(const String& url, HttpMethod method, const String& payload = "") {
//     HTTPClient http;
//     int httpResponseCode = 0;

//     switch (method) {
//         case GET:
//             http.begin(url);
//             httpResponseCode = http.GET();
//             break;
//         case POST:
//             http.begin(url);
//             http.addHeader("Content-Type", "application/json");
//             httpResponseCode = http.POST(payload);
//             break;
//         case PUT:
//             http.begin(url);
//             http.addHeader("Content-Type", "application/json");
//             httpResponseCode = http.PUT(payload);
//             break;
//         case DELETE:
//             http.begin(url);
//             httpResponseCode = http.clear();
//             break;
//         case PATCH:
//             http.begin(url);
//             http.addHeader("Content-Type", "application/json");
//             httpResponseCode = http.PATCH(payload);
//             break;
//     }

//     if (httpResponseCode > 0) {
//         Serial.print("HTTP Response code: ");
//         Serial.println(httpResponseCode);
//     } else {
//         Serial.print("Error en la solicitud HTTP: ");
//         Serial.println(httpResponseCode);
//     }

//     http.end();
// }
