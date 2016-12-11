<div>
    <table class="table table-striped">
        <thead>
            <tr>
                <th style="text-align: center">Type</th>
                <th style="text-align: center">Date & Time</th>
                <th style="text-align: center">Symbol</th>
                <th style="text-align: center">Shares</th>
                <th style="text-align: center">Price</th>
                <th style="text-align: center">Transaction Value</th>
            </tr>
        </thead>
        <?php foreach($transactions as $transaction): ?>
            <tr>
                <td><?= $transaction["type"] ?></td>
                <td><?= $transaction["date_time"] ?></td>
                <td><?= $transaction["symbol"] ?></td>
                <td><?= $transaction["shares"] ?></td>
                <td>$<?= $transaction["price"] ?></td>
                <td>$<?= $transaction["value"] ?></td>
            </tr>
        <?php endforeach ?>
    </table>
</div>
