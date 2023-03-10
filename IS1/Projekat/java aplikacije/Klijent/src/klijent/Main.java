package klijent;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.stream.Collectors;

public class Main {
    private static final Scanner sc = new Scanner(System.in);
    
    private static String serializeParams(Map<String, String> args) {
        return args.entrySet().stream()
            .map(entry -> entry.getKey() + '=' + entry.getValue())
            .collect(Collectors.joining("&"));
    }
    
    private static void sendRequest(
        String resourceUrl, String method,
        Map<String, String> queryArgs, Map<String, String> bodyArgs, boolean auth
    ) throws MalformedURLException, IOException {
        String stringUrl = "http://localhost:8080/CentralniServer/resources/" + resourceUrl;
        
        if(queryArgs != null)
            stringUrl += '?' + serializeParams(queryArgs);
        
        URL url = new URL(stringUrl);
        
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod(method);
        
        if (auth) {
            System.out.println("Korisničko ime: ");
            String username = sc.next();
            
            System.out.println("Šifra: ");
            String password = sc.next();
            
            String credentials = username + ':' + password;
            String encodedCredentials = Base64.getEncoder().encodeToString(credentials.getBytes(StandardCharsets.UTF_8));
            
            String authString = "Basic " + encodedCredentials;
            
            connection.setRequestProperty("Authorization", authString);
        }
        
        if (bodyArgs != null) {
            connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
            connection.setDoOutput(true);
            
            byte[] serializedArgs = serializeParams(bodyArgs).getBytes(StandardCharsets.UTF_8);
            
            connection.setRequestProperty("Content-Length", Integer.toString(serializedArgs.length));
            
            try (DataOutputStream os = new DataOutputStream(connection.getOutputStream())) {
                os.write(serializedArgs);
                //os.flush();
            }
        }
        
        int responseCode = connection.getResponseCode();
        System.out.println("Response code: " + responseCode);
        
        StringBuilder response;
        try (BufferedReader in = new BufferedReader(new InputStreamReader(responseCode < 400 ? connection.getInputStream() : connection.getErrorStream()))) {
            response = new StringBuilder();
            String inputLine;
            while ((inputLine = in.readLine()) != null)
                response.append(inputLine);
        }

        System.out.println(response.toString());
    }
    
    public static void cityRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Kreiranje");
        System.out.println("2) Ispis svih");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Ime: ");
            sc.skip("\\R?");
            bodyArgs.put("ime", sc.nextLine());
            
            sendRequest("gradovi", "POST", null, bodyArgs, false);
            
            break;
        }
        case 2: {
            sendRequest("gradovi", "GET", null, null, false);
            
            break;
        }
        }
    }
    
    public static void userRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Kreiranje");
        System.out.println("2) Dodavanje novca");
        System.out.println("3) Promjena adrese");
        System.out.println("4) Ispis svih");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Korisnicko ime: ");
            bodyArgs.put("korisnickoIme", sc.next());
            
            System.out.print("Sifra: ");
            bodyArgs.put("sifra", sc.next());
            
            System.out.print("Ime: ");
            bodyArgs.put("ime", sc.next());
            
            System.out.print("Prezime: ");
            bodyArgs.put("prezime", sc.next());
            
            System.out.print("Adresa: ");
            sc.skip("\\R?");
            bodyArgs.put("adresa", sc.nextLine());
            
            System.out.print("Grad: ");
            sc.skip("\\R?");
            bodyArgs.put("grad", sc.nextLine());
            
            
            sendRequest("korisnici", "POST", null, bodyArgs, false);
            
            break;
        }
        case 2: {
            System.out.print("Korisnicko ime: ");
            String username = sc.next();
            
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Dodatni novac: ");
            bodyArgs.put("dodatniNovac", sc.next());
        
            sendRequest(String.format("korisnici/%s/novac", username), "PUT", null, bodyArgs, false);
            
            break;
        }
        case 3: {
            System.out.print("Korisnicko ime: ");
            String username = sc.next();
            
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Adresa: ");
            sc.skip("\\R?");
            bodyArgs.put("adresa", sc.nextLine());
            
            System.out.print("Grad: ");
            sc.skip("\\R?");
            bodyArgs.put("grad", sc.nextLine());
        
            sendRequest(String.format("korisnici/%s/adresa", username), "PUT", null, bodyArgs, false);
            
            break;
        }
        case 4: {
            sendRequest("korisnici", "GET", null, null, false);
            
            break;
        }
        }
    }
    
    public static void categoryRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Kreiranje");
        System.out.println("2) Ispis svih");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Ime: ");
            sc.skip("\\R?");
            bodyArgs.put("ime", sc.nextLine());
            
            System.out.print("Natkategorija: ");
            sc.skip("\\R?");
            bodyArgs.put("natkategorija", sc.nextLine());
            
            sendRequest("kategorije", "POST", null, bodyArgs, false);
            
            break;
        }
        case 2: {
            sendRequest("kategorije", "GET", null, null, false);
            
            break;
        }
        }  
    }
    
    public static void itemRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Kreiranje");
        System.out.println("2) Promjena cijene");
        System.out.println("3) Promjena popusta");
        System.out.println("4) Ispis svih koje prodaje korisnik");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Ime: ");
            sc.skip("\\R?");
            bodyArgs.put("ime", sc.nextLine());
            
            System.out.print("Opis: ");
            sc.skip("\\R?");
            bodyArgs.put("opis", sc.nextLine());
            
            System.out.print("Cena: ");
            bodyArgs.put("cena", sc.next());
            
            System.out.print("Popust: ");
            bodyArgs.put("popust", sc.next());
            
            System.out.print("Prodavac: ");
            bodyArgs.put("prodavac", sc.next());
            
            System.out.print("Kategorija: ");
            sc.skip("\\R?");
            bodyArgs.put("kategorija", sc.nextLine());
            
            
            sendRequest("artikli", "POST", null, bodyArgs, false);
            
            break;
        }
        case 2: {
            System.out.print("ID: ");
            int id = sc.nextInt();
            
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Nova cijena: ");
            bodyArgs.put("cijena", sc.next());
        
            sendRequest(String.format("artikli/%d/cijena", id), "PUT", null, bodyArgs, false);
            
            break;
        }
        case 3: {
            System.out.print("ID: ");
            int id = sc.nextInt();
            
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("Novi popust: ");
            bodyArgs.put("popust", sc.next());
        
            sendRequest(String.format("artikli/%d/popust", id), "PUT", null, bodyArgs, false);
            
            break;
        }
        case 4: {
            sendRequest("artikli", "GET", null, null, true);
            
            break;
        }
        }
    }
    
    public static void cartRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Dodaj artikal");
        System.out.println("2) Izbaci artikal");
        System.out.println("3) Prikazi");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {
            Map<String, String> bodyArgs = new HashMap<>();
            
            System.out.print("ID artikla: ");
            bodyArgs.put("artikal", sc.next());
            
            System.out.print("Kolicina: ");
            sc.skip("\\R?");
            bodyArgs.put("kolicina", sc.next());            
            
            sendRequest("korpa", "POST", null, bodyArgs, true);
            
            break;
        }
        case 2: {            
            System.out.print("ID artikla: ");
            int itemId = sc.nextInt();
        
            sendRequest(String.format("korpa/%d", itemId), "DELETE", null, null, true);
            
            break;
        }
        case 3: {
            sendRequest("korpa", "GET", null, null, true);
            
            break;
        }
        }
    }
    
    public static void orderRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Naruci iz korpe");
        System.out.println("2) Prikazi od korisnika");
        System.out.println("3) Prikazi sve");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {   
            sendRequest("narudzbine", "POST", null, null, true);
            
            break;
        }
        case 2: {            
            sendRequest("narudzbine/korisnik", "GET", null, null, true);
            
            break;
        }
        case 3: {
            sendRequest("narudzbine", "GET", null, null, false);
            
            break;
        }
        }
    }
    
     public static void transactionRequests() throws IOException {
        System.out.println("Odaberite operaciju: ");
        System.out.println("1) Prikazi sve");
        
        int choice = sc.nextInt();
        
        switch(choice) {
        case 1: {
            sendRequest("transakcije", "GET", null, null, false);
            
            break;
        }
        }
    }
    
    public static void main(String[] args) throws IOException {
        while(true) {
            System.out.println("Odaberite resurs: ");
            System.out.println("1) Gradovi");
            System.out.println("2) Korisnici");
            System.out.println("3) Kategorije");
            System.out.println("4) Artikli");
            System.out.println("5) Korpa");
            System.out.println("6) Narudzbine");
            System.out.println("7) Transakcije");
            
            int choice = sc.nextInt();
            
            switch(choice) {
            case 1:
                cityRequests();
                break;
            case 2:
                userRequests();
                break;
            case 3:
                categoryRequests();
                break;
            case 4:
                itemRequests();
                break;
            case 5:
                cartRequests();
                break;
            case 6:
                orderRequests();
                break;
            case 7:
                transactionRequests();
                break;
            }
        }
    }
}
