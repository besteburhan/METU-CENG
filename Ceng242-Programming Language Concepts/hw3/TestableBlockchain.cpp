#include <sstream>
#include "TestableBlockchain.h"
#include "Blockchain.h"

void TestableBlockchain::runInternal() {

    // Phase 1 : Constructor

    Blockchain blockchain1(1);
    Blockchain blockchain2(2);

    Koin *head1 = blockchain1.getHead();
    Koin *head2 = blockchain2.getHead();

    if (blockchain1.getID() != 1 || blockchain2.getID() != 2 || head1 != nullptr || head2 != nullptr)
        currentGrade -= 1;
    std::cout << currentGrade << std::endl;////1

    Koin *head3 = new Koin(0.5);

    Blockchain blockchain3(3, head3);

    Koin *actualHead3 = blockchain3.getHead();

    if (head3 == nullptr || !verifyKoinValues(*head3, *actualHead3))
        currentGrade -= 1;
    std::cout << currentGrade << std::endl;////2

    // Phase 2 : Copy Constructor

    Blockchain blockchain4(blockchain3);

    Koin *head4 = blockchain4.getHead();

    if (head3 == nullptr || !verifyKoinValues(*head4, *actualHead3))
        currentGrade -= 2;
    std::cout << currentGrade << std::endl;////3

    // Phase 3 : Move

    Blockchain blockchain5(blockchain4);

    blockchain5 = std::move(blockchain3);

    // Phase 4 : Operations

    int koinCount = 10;

    for(int i = 0; i < koinCount; i++) {
        ++blockchain1;
    }

    double actualValue = 6.0434;

    if (!verifyTwoValues(actualValue, blockchain1.getTotalValue()))
        currentGrade -= 1;
    std::cout << blockchain1 << std::endl;
  std::cout << currentGrade << std::endl;
    blockchain1 *= 10;
        std::cout << blockchain1 << std::endl;


    if (!verifyTwoValues(actualValue * 10, blockchain1.getTotalValue()))
        currentGrade -= 1;

    blockchain1 /= 10;

    if (!verifyTwoValues(actualValue, blockchain1.getTotalValue()))
        currentGrade -= 1;
    std::cout <<"sdfgh"<< currentGrade << std::endl;

    long chainLength = blockchain1.getChainLength();

    if (chainLength != koinCount){
            currentGrade -= 1;
        std::cout <<"-1" << currentGrade << std::endl;
        std::cout << blockchain1 << std::endl;////2
        std::cout << koinCount << std::endl;////2
        std::cout << blockchain1.getChainLength() << std::endl;////2
    }

    for(int i = 0; i < 100; i++) {
        --blockchain1;
    }
std::cout << blockchain1 << std::endl;////2
    if (!verifyTwoValues(0, blockchain1.getTotalValue()))
        currentGrade -= 1;

    chainLength = blockchain1.getChainLength();

    if (chainLength != 0)
        currentGrade -= 1;

    // Phase 5 : Stream Overload

    for(int i = 0; i < 5; i++) {
        ++blockchain2;
    }

    std::stringstream sstream;
    sstream << blockchain1;

    std::string blockchain1Result = "Block 1: Empty.";

    if (blockchain1Result != sstream.str())
        currentGrade -= 2;

    sstream.str(std::string()); // Clear the stream
    sstream << blockchain2;

    std::string blockchain2Result = "Block 2: 0.965--0.047--0.532--0.385--0.883--|(2.811)";
    std::cout << blockchain2 << std::endl;

    if (blockchain2Result != sstream.str())
        currentGrade -= 2;
}
