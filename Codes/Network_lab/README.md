
# Experimentos de Rede — Tempo Real, NTP e TCP

Este pacote demonstra, de forma prática, os principais conceitos de redes de computadores:
- Comunicação **tempo real (soft real-time)** e deadlines.
- Diferença de relógios e **sincronização de tempo** (offset e drift).
- Funcionamento de **protocolos UDP e TCP**.
- **NTP (Network Time Protocol)** e estimativa de delay/offset.

---

## 1. Tempo Real via UDP
**Arquivos:** `01_realtime_udp_sender.py` e `01_realtime_udp_receiver.py`

### Conceito
Demonstra **soft real-time**, onde mensagens devem ser processadas dentro de um prazo (*deadline*).

### Funcionamento
- O sender envia pacotes UDP periodicamente (ex.: 50 Hz = 20 ms por envio).
- O receiver mede a **latência** e contabiliza mensagens que violam o prazo.

### Parâmetros importantes
- `--rate_hz`: frequência de envio.
- `--jitter_ms`: variação aleatória no envio.
- `--work_ms`: simula carga de CPU.
- `--deadline_ms`: tempo máximo aceitável.

| Termo | Significado |
|--------|--------------|
| Latência | Tempo entre envio e recebimento |
| Jitter | Variação do tempo de entrega |
| Deadline violation | Mensagem processada fora do tempo limite |

---

## 2. Diferença de relógios (SoftClock)
**Arquivo:** `02_softclock.py`

### Conceito
Simula dois nós (A e B) com relógios independentes, cada um com **offset** (atraso/adianto) e **drift (ppm)** — ou seja, velocidades diferentes.

### Funcionamento
- Cada nó usa um relógio de software com offset e drift configurável.
- Trocam mensagens UDP e exibem seus tempos locais e de software.

### Objetivo
Mostrar como dois computadores não mantêm sincronismo natural de tempo sem correção periódica.

---

## 3. Mini NTP (UDP)
**Arquivos:** `03_sync_server.py` e `03_sync_client.py`

### Conceito
Reproduz o cálculo de **offset** e **delay** do protocolo NTP.

| Variável | Descrição |
|-----------|------------|
| T1 | Cliente envia requisição |
| T2 | Servidor recebe |
| T3 | Servidor envia resposta |
| T4 | Cliente recebe resposta |

Cálculos:
```
delay = (T4 - T1) - (T3 - T2)
offset = ((T2 - T1) + (T3 - T4)) / 2
```

### Resultado esperado
Offsets estáveis (ms) e delays variáveis conforme latência de rede.

---

## 4. Consulta a servidores NTP reais
**Arquivo:** `04_ntp_query.py`

Consulta servidores NTP públicos como `time.google.com` e `pool.ntp.org`.

Mostra **offset**, **delay**, **jitter** e **stratum** (camada hierárquica).

---

## 5. TCP Echo e RTT
**Arquivos:** `05_tcp_echo_server.py` e `05_tcp_client_rtt.py`

### Conceito
Mede **latência (RTT)** via TCP, comparando efeito do **Nagle Algorithm**.

### Funcionamento
- Servidor ecoa mensagens recebidas.
- Cliente mede tempo de ida e volta (RTT).

Ative `--nodelay` no cliente para desativar o algoritmo de Nagle e reduzir latência em pacotes pequenos.

---

## 6. Throughput TCP
**Arquivos:** `06_tcp_throughput_server.py` e `06_tcp_throughput_client.py`

### Conceito
Mede **taxa de transferência (MB/s)** entre cliente e servidor TCP.

### Funcionamento
- Cliente envia um volume fixo de dados (ex.: 100 MB).
- Servidor soma bytes recebidos e calcula velocidade média.

| Parâmetro | Efeito |
|------------|--------|
| `--chunk_kb` | Tamanho dos blocos enviados |
| `--nodelay` | Desativa buffering do TCP (pode afetar pequenos blocos) |

---

## 7. Sincronização T1..T4 via TCP
**Arquivos:** `07_tcp_time_sync_server.py` e `07_tcp_time_sync_client.py`

Versão TCP do mini-NTP, calculando delay e offset com conexão persistente (sem perdas).

TCP garante entrega e ordem → resultados mais estáveis e realistas.

---

## 8. Resumo conceitual

| Conceito | Experimento | Métrica | Aplicação |
|-----------|--------------|---------|------------|
| Soft real-time | UDP sender/receiver | Latência, deadline | Sistemas de tempo real |
| Sincronização | SoftClock / NTP | Offset, drift | Sincronização de relógios |
| Transporte | TCP vs UDP | RTT, jitter | Camadas de transporte |
| Throughput | TCP throughput | MB/s | Teste de desempenho |
| NTP real | ntplib | Offset real | Sincronização global |

---

## Sugestões de uso
- Plotar gráficos de offset/latência ao longo do tempo.
- Demonstrar jitter e carga CPU.
- Comparar TCP e UDP quanto a tempo real e confiabilidade.
- Usar consulta NTP para discutir sincronização distribuída.

## Como usar no Codespaces

1. Suba estes arquivos para um repositório e abra no **GitHub Codespaces**.
2. (Opcional) Ambiente virtual e dependências:
   ```bash
   python -m venv .venv && source .venv/bin/activate
   pip install -r requirements.txt
   ```

## 1) Tempo real (UDP)

Terminal A:
```bash
python 01_realtime_udp_receiver.py --port 5005 --deadline_ms 30 --work_ms 2
```

Terminal B:
```bash
python 01_realtime_udp_sender.py --host 127.0.0.1 --port 5005 --rate_hz 50 --jitter_ms 8
```

## 2) Diferença de relógios (soft clock)

Terminal A:
```bash
python 02_softclock.py --name A --port 6001 --offset_ms +120 --ppm +30
```

Terminal B:
```bash
python 02_softclock.py --name B --port 6002 --offset_ms -250 --ppm -50 --peer 127.0.0.1:6001
```

## 3) Mini-NTP + NTP real

Servidor:
```bash
python 03_sync_server.py --port 7001
```

Cliente:
```bash
python 03_sync_client.py --host 127.0.0.1 --port 7001 --samples 10 --sleep_ms 500 --fake_offset_ms +200 --fake_ppm +40
```

Consulta NTP real (se permitido pela rede):
```bash
python 04_ntp_query.py --hosts pool.ntp.org time.google.com time.cloudflare.com --repeat 5 --sleep_s 1.0
```

## 4) Demos TCP

### 4.1 RTT (eco) e Nagle
Servidor:
```bash
python 05_tcp_echo_server.py --port 8001 --work_ms 0 --delay_ms 0
```
Cliente:
```bash
python 05_tcp_client_rtt.py --host 127.0.0.1 --port 8001 --count 500 --payload_bytes 64 --rate_hz 100 --nodelay
```
> Remova `--nodelay` para observar efeito do Nagle.

### 4.2 Throughput TCP
Servidor:
```bash
python 06_tcp_throughput_server.py --port 8002
```
Cliente:
```bash
python 06_tcp_throughput_client.py --host 127.0.0.1 --port 8002 --megs 100 --chunk_kb 64 --nodelay
```

### 4.3 Sincronização T1..T4 via TCP
Servidor:
```bash
python 07_tcp_time_sync_server.py --port 8003
```
Cliente:
```bash
python 07_tcp_time_sync_client.py --host 127.0.0.1 --port 8003 --samples 10 --sleep_ms 300 --fake_offset_ms +150 --fake_ppm +20 --nodelay
```
