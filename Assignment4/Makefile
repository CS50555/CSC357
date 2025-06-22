CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGETS= keygen otp_enc_d otp_enc otp_dec otp_dec_d
 
all: $(TARGETS)

keygen: keygen.c
	$(CC) $(CFLAGS) -o keygen keygen.c

opt_enc: otp_enc.c
	$(CC) $(CFLAGS) -o otp_enc opt_enc.c

opt_edec: otp_dec.c
	$(CC) $(CFLAGS) -o otp_dec opt_dec.c

opt_enc_d: otp_enc_d.c
	$(CC) $(CFLAGS) -o otp_enc_d opt_enc_d.c

opt_dec_d: otp_dec_d.c
	$(CC) $(CFLAGS) -o otp_dec_d opt_dec_d.c

clean:
	rm -f $(TARGETS)