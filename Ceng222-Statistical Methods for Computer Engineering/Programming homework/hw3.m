N=4145;
lambda_Mot = 40;
lambda_Aut = 30;
lambda_Tru = 20;
totalWeight = zeros(N,1);

for k=1:N
    U = rand; i=0;
    F = exp(-lambda_Mot);
    while (U>=F)
        i=i+1;
        F = F + exp(-lambda_Mot)*lambda_Mot^i/gamma(i+1);
    end
    numberMot = i;
    lambda_W_Mot = 0.15;
    alphaMot = 16;
    weightMot = sum(-1/lambda_W_Mot * log(rand(alphaMot,numberMot)));
    totalW_Mot = sum(weightMot);
    
    U = rand; i=0;
    F = exp(-lambda_Aut);
    while (U>=F)
        i=i+1;
        F = F + exp(-lambda_Aut)*lambda_Aut^i/gamma(i+1);
    end
    numberAut = i;
    lambda_W_Aut = 0.05;
    alphaAut = 60;
    weightAut = sum(-1/lambda_W_Aut * log(rand(alphaAut,numberAut)));
    totalW_Aut = sum(weightAut);
    
    U = rand; i=0;
    F = exp(-lambda_Tru);
    while (U>=F)
        i=i+1;
        F = F + exp(-lambda_Tru)*lambda_Tru^i/gamma(i+1);
    end
    numberTru = i;
    lambda_W_Tru = 0.01;
    alphaTru = 84;
    weightTru = sum(-1/lambda_W_Tru * log(rand(alphaTru,numberTru)));
    totalW_Tru = sum(weightTru);
    
    totalWeight(k) = totalW_Mot + totalW_Aut + totalW_Tru;
    
end

p_est = mean(totalWeight>220000);
expectedWeight = mean(totalWeight);
stdWeight = std(totalWeight);
fprintf('Estimated probability = %f\n',p_est);
fprintf('Expected weight = %f\n',expectedWeight);
fprintf('Standard deviation = %f\n',stdWeight);