clc, clear all, close all

Ts = 1/1000; % secCALIBRAonds
Vd = 60; % voltage
Vdc = Vd;
R = 22.5;  %127 % ohms
L = 0.10 ; % henry
x_ref = [0.5 0.5]' ;

%**************************************************************************
%                       INIT SYSTEM PARAMETERS
%**************************************************************************
% transformada clark
K = 2/3 * [1 -1/2 -1/2; 0 sqrt(3)/2 -sqrt(3)/2] ;

I_2 = eye(2);

% state matrix
F = - R/L * I_2;
G = (Vd/(2*L)) * K;
A = expm(F*Ts); % matrix exponential
B = -F^(-1) * (I_2 - A) * G;

lambda = 0.01; % switch
lambda_i = 10 ; % current


uk=[0 0 0]';

switch_state = [
    0 0 0;
    0 0 1;
    0 1 0;
    0 1 1;
    1 0 0;
    1 1 0;
    1 0 1;
    1 1 1;
];

n = (size(switch_state));
n = n(1);
np=1;


%**************************************************************************
%**************************************************************************
%**************************************************************************
%**************************************************************************
%
% POSSIBILITIES
%
%**************************************************************************
% Condicao 0 inicial
x0 = [0 ; 0];
i_alfa = x0(1); i_beta = x0(2);
u0 = [0 ; 0 ; 0];
i_alfabeta = x0;
%**************************************************************************

k_test = 30;
x = cell(k_test,1);
x{1} = x0;
u = cell(k_test,1);
x{2} = x0;

u{1} = u0;
index_min_all = zeros(k_test-1,1);
g_i_all = zeros(n,1)
for k = 2 : k_test
        g=zeros(n,1);

        for possibilidade=1:n
            disp ("A * x{k} ")
            disp(A * x{k} )
            disp ("B * u{k} ")
            disp(B * switch_state(possibilidade,1:end)'  )
            x_p = A * x{k} + B * switch_state(possibilidade,1:end)' ;

            g_u = norm([u{k-1} - switch_state(possibilidade,1:end)']) ;

            g_i = norm(x_ref - x_p) ;

            g(possibilidade) =  lambda * g_u + lambda_i * g_i ;
        end

        index_min_all(k) = find(g==min(g)) ;
        gi_all(k) = g(find(g==min(g))) ;
        u{k} = switch_state( find(g==min(g)) , 1:end )' ;
        x{k+1} = A * x{k} + B * u{k};
        disp(x{k})

end

i_alfa = zeros(k_test, 1);
i_beta = zeros(k_test, 1);
i_a_all = zeros(k_test, 1);
i_b_all = zeros(k_test, 1);
i_c_all = zeros(k_test, 1);
i_abc=  zeros(k_test, 3);
ua = zeros(k_test, 1);
ub = zeros(k_test, 1);
uc = zeros(k_test, 1);
vf_all= zeros(k_test, 1);
i_all = cell2mat(x);
u_all = cell2mat(u);

cont = 1;
for i = 3:2:size(i_all)
    i_alfa(cont) = i_all(i);
    cont = cont + 1;
end
cont = 1;
for i = 4:2:size(i_all)
    i_beta(cont) = i_all(i);
    cont = cont + 1;
end
for i = 1:k_test
    ik = K * [1, 0 ; 0, 1; -1, -1] * [i_alfa(i); i_beta(i)]
    ia = ik(1)
    ib = ik(2)
    ic= ia-ib;

    i_a_all(i) = ia ;
    i_b_all(i) = ib ;
    i_c_all(i) = ic;
    i_abc(i,1:3) = [ia ; ib ; ic ];
end
cont = 1;
for i = 1:3:size(u_all)
    ua(cont) = u_all(i);
    cont = cont + 1;
end
cont = 1;
for i = 2:3:size(u_all)
    ub(cont) = u_all(i);
    cont = cont + 1;
end
cont = 1;
for i = 3:3:size(u_all)
    uc(i) = u_all(i);
    cont = cont + 1;
end
for i = 1:k_test
    vf_all(i) = 2/3 * (Vdc/2 * ua(i) * exp(j*0) + Vdc/2 * ub(i) * exp(j*2/3) + Vdc/2 * uc(i) * exp(j*4*pi/3) );
end




% plot control
figure
x=5;
colors = {'r', 'g', 'b', 'm', 'c', 'y', 'k', 'r', 'g'};
hold on;
xlabel('k'), ylabel(' I [A] ');
legend('x_1(t)', 'x_2(t)')
sgtitle('Plot \lambda = 0.01 ; \lambda_i = 1 ; x_{ref} = [.5 .5]; T_s = 5e-05');

subplot(x,1,1);
plot(i_a_all, colors{5}); hold on;
plot(i_b_all, colors{4}); hold on;
legend('ia_{leg A}', 'ib_{leg B} ')
ylabel('A');
grid on;

subplot(x,1,2);
plot(i_alfa, colors{5});hold on;
plot(i_beta, colors{4});hold on;

legend('i alfa ', 'i beta ')
xlabel('k'), ylabel(' I [A] ')
grid on;

subplot(x,1,3);
plot(ua, colors{1}); hold on;
xlabel('k'), ylabel(' switch A ')

subplot(x,1,4);
plot(ub, colors{2}); hold on;
xlabel('k'), ylabel(' switch B ')

subplot(x,1,5);
plot(uc, colors{3}); hold on;
xlabel('k'), ylabel(' switch C ')

axis([-0.5 length(ua)+1 -0.1 1.1]);
grid on;

% plot indice


% plot com penalizacao
% eigen

