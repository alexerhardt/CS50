<div>
    <table class="table table-striped">
        <thead>
            <tr>
                <th style="text-align: center">Symbol</th>
                <th style="text-align: center">Name</th>
                <th style="text-align: center">Shares</th>
                <th style="text-align: center">Price</th>
                <th style="text-align: center">Value</th>
            </tr>
        </thead>
        <?php foreach($positions as $position): ?>
            <tr>
                <td><?= $position["symbol"] ?></td>
                <td><?= $position["name"] ?></td>
                <td><?= $position["shares"] ?></td>
                <td>$<?= $position["price"] ?></td>
                <td>$<?= $position["value"] ?></td>
            </tr>
        <?php endforeach ?>
            <tr>
                <td>CASH</td>
                <td></td>
                <td></td>
                <td></td>
                <td>$<?= $cash ?></td>
            </tr>
            <tr style="background-color: #373A3C">
                <td></td>
                <td><p style="color: white"><b>TOTAL VALUE OF PORTFOLIO</b></p></td>
                <td></td>
                <td></td>
                <td><p style="color: white"><b>$<?= $total ?></b></p></td>
            </tr>
    </table>
</div>
